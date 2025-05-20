/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:43:01 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/20 22:18:20 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h"

extern int	g_tmp;

int	read_input_to_pipe(char *delimiter, bool expand,
								t_data *data, int pipe_write_fd)
{
	char	*line;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (TRUE)
	{
		if (g_tmp == 3)
			return (130);
		line = readline("> ");
		if (g_tmp == 3)
		{
			if (line)
				(free(line), line = NULL);
			return (130);
		}
		if (!line)
			return (0);
		if (handle_delimiter(line, delimiter, delim_len))
			break ;
		if (process_line(line, expand, data, pipe_write_fd) < 0)
			return (-1);
	}
	return (0);
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

int	process_heredocs(t_cmd *cmd_list, t_data *data)
{
	int	saved_stdin_fd;
	int	overall_status;

	if (count_total_heredocs(cmd_list) > MAX_HEREDOCS)
	{
		ft_putstr_fd("minishell: max here-document count exceeded\n", 2);
		//TODO: check if leaks or not before exit
			// ft_cmd_clear(&command_list);
			// ft_tenv_clear(&data.env_list);
			// rl_clear_history();
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
