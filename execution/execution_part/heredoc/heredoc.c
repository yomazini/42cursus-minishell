/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:43:01 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/19 14:54:15 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h"

extern int	g_tmp;

static int	handle_signal_or_eof(char *line)
{
	if (g_tmp == 3)
	{
		if (line)
			free(line);
		return (130);
	}
	if (!line)
		return (0);
	return (-1);
}

static int	handle_delimiter(char *line, char *delimiter, size_t len)
{
	if (ft_strncmp(line, delimiter, len + 1) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static int	write_and_free(int fd, char *line_to_write, char *line, bool expand)
{
	if (write(fd, line_to_write, ft_strlen(line_to_write)) < 0
		|| write(fd, "\n", 1) < 0)
	{
		perror("minishell: heredoc: write to pipe failed");
		if (expand && line_to_write != line)
			free(line_to_write);
		else if (!expand)
			free(line);
		return (-1);
	}
	if (expand && line_to_write != line)
		free(line_to_write);
	else if (!expand)
		free(line);
	return (0);
}

static int	process_line(char *line, bool expand, t_data *data,
						int pipe_write_fd)
{
	char	*line_to_write;

	if (expand)
	{
		line_to_write = expand_heredoc_line(line, data);
		free(line);
		if (!line_to_write)
			return (-1);
	}
	else
		line_to_write = line;
	return (write_and_free(pipe_write_fd, line_to_write, line, expand));
}

int	read_input_to_pipe(char *delimiter, bool expand,
						t_data *data, int pipe_write_fd)
{
	char	*line;
	size_t	delim_len;
	int		status;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		if (g_tmp == 3)
			return (130);
		line = readline("> ");
		status = handle_signal_or_eof(line);
		if (status != -1)
			return (status);
		if (handle_delimiter(line, delimiter, delim_len))
			break ;
		if (process_line(line, expand, data, pipe_write_fd) < 0)
			return (-1);
	}
	return (0);
}

void	cleanup_all_heredoc_fds(t_cmd *cmd_list)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		current_redir = current_cmd->redir;
		while (current_redir)
		{
			if (current_redir->type == TOKEN_REDIR_HEREDOC
				&& current_redir->heredoc_fd != -1)
			{
				close(current_redir->heredoc_fd);
				current_redir->heredoc_fd = -1;
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}

static int	count_total_heredocs(t_cmd *cmd_list)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;
	int		count;

	current_cmd = cmd_list;
	count = 0;
	while (current_cmd)
	{
		current_redir = current_cmd->redir;
		while (current_redir)
		{
			if (current_redir->type == TOKEN_REDIR_HEREDOC)
				count++;
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (count);
}

static	int	init_heredoc(t_data *data, int *saved_stdin_fd)
{
	*saved_stdin_fd = dup(STDIN_FILENO);
	if (*saved_stdin_fd < 0)
	{
		perror("minishell: dup (saving stdin for heredoc)");
		set_signal_handlers_prompt();
		data->last_exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	g_tmp = 0;
	set_signal_handlers_heredoc();
	return (EXIT_SUCCESS);
}

static int	handle_one_heredoc(t_redir *r, t_data *data, int *pipe_fds)
{
	int	status;

	if (pipe(pipe_fds) < 0)
	{
		perror("minishell: heredoc pipe");
		return (EXIT_FAILURE);
	}
	status = read_input_to_pipe(r->filename, r->expand_heredoc,
			data, pipe_fds[1]);
	close(pipe_fds[1]);
	if (g_tmp == 3 || status == 130 || status < 0)
	{
		close(pipe_fds[0]);
		return (EXIT_FAILURE);
	}
	r->heredoc_fd = pipe_fds[0];
	return (EXIT_SUCCESS);
}

static int	loop_heredocs(t_cmd *cmd, t_data *data)
{
	t_redir	*redir;
	int		pipe_fds[2];
	int		ret;

	while (cmd && g_tmp != 3)
	{
		redir = cmd->redir;
		while (redir && g_tmp != 3)
		{
			if (redir->type == TOKEN_REDIR_HEREDOC
				&& redir->heredoc_fd < 0)
			{
				ret = handle_one_heredoc(redir, data, pipe_fds);
				if (ret != EXIT_SUCCESS)
					return (ret);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

static void	restore_after_heredoc(int saved_stdin_fd)
{
	set_signal_handlers_prompt();
	if (saved_stdin_fd >= 0)
	{
		if (dup2(saved_stdin_fd, STDIN_FILENO) < 0)
			perror("minishell: dup2 (restoring stdin after heredoc)");
		close(saved_stdin_fd);
	}
}

int	process_heredocs(t_cmd *cmd_list, t_data *data)
{
	int	saved_stdin_fd;
	int	overall_status;

	if (count_total_heredocs(cmd_list) > MAX_HEREDOCS)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n",
			STDERR_FILENO);
		exit(2);
	}
	if (init_heredoc(data, &saved_stdin_fd) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	overall_status = loop_heredocs(cmd_list, data);
	restore_after_heredoc(saved_stdin_fd);
	if (g_tmp == 3)
	{
		data->last_exit_status = 130;
		cleanup_all_heredoc_fds(cmd_list);
		return (EXIT_FAILURE);
	}
	if (overall_status != EXIT_SUCCESS)
	{
		data->last_exit_status = EXIT_FAILURE;
		cleanup_all_heredoc_fds(cmd_list);
	}
	return (overall_status);
}
