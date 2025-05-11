/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:43:01 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/11 10:03:49 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h"

extern volatile sig_atomic_t	g_received_signal;
char							*expand_heredoc_line(char *line, t_data *data);
void							set_signal_handlers_prompt(void); 
static int	read_input_to_pipe(char *delimiter, bool expand,
								t_data *data, int pipe_write_fd)
{
	char	*line;
	char	*line_to_write;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		if (g_received_signal == SIGINT)
			return (130); // Signal interruption

		line = readline("> "); // Heredoc prompt

		// Check signal flag *after* readline (handler might have run)
		if (g_received_signal == SIGINT)
		{
			free(line); // Readline might have allocated something
			return (130); // Signal interruption
		}

		if (!line) // Actual EOF (Ctrl+D)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
			ft_putstr_fd("end-of-file (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			return (0); // EOF is not an error for read_input, just end of input
		}
		if (ft_strncmp(line, delimiter, delim_len + 1) == 0)
		{
			free(line);
			break ; // Delimiter found
		}
		if (expand) { 
					line_to_write = expand_heredoc_line(line, data);
			free(line); // Free original readline buffer
			if (!line_to_write) return (-1); // Malloc error in expander

			/* ... expansion ... */ } else { line_to_write = line; }
		if (write(pipe_write_fd, line_to_write, ft_strlen(line_to_write)) < 0
			|| write(pipe_write_fd, "\n", 1) < 0) {
			perror("minishell: heredoc: write to pipe failed");
			if (expand && line_to_write != line) free(line_to_write); else if (!expand) free(line);
			return (-1); // Critical write error
		}
		if (expand && line_to_write != line) free(line_to_write); else if (!expand) free(line);
	}
	return (0); // Success
}


void cleanup_all_heredoc_fds(t_cmd *cmd_list)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		current_redir = current_cmd->redir;
		while (current_redir)
		{
			if (current_redir->type == TOKEN_REDIR_HEREDOC && current_redir->heredoc_fd != -1)
			{
				close(current_redir->heredoc_fd);
				current_redir->heredoc_fd = -1; // Mark as closed
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}

int	process_heredocs(t_cmd *cmd_list, t_data *data)
{
	t_cmd				*current_cmd;
	t_redir				*current_redir;
	int					pipe_fds[2];
	int					read_status_code;

	set_signal_handlers_heredoc();
	g_received_signal = 0; 

	current_cmd = cmd_list;
	while (current_cmd != NULL)
	{
		if (g_received_signal == SIGINT) break;
		current_redir = current_cmd->redir;
		while (current_redir != NULL)
		{
			if (g_received_signal == SIGINT) break;
			if (current_redir->type == TOKEN_REDIR_HEREDOC)
			{
				if (current_redir->heredoc_fd == -1) 
				{
					if (pipe(pipe_fds) == -1) {
						perror("minishell: heredoc pipe");
						data->last_exit_status = EXIT_FAILURE;
						set_signal_handlers_prompt(); 
						cleanup_all_heredoc_fds(cmd_list); 
						return (EXIT_FAILURE);
					}
					read_status_code = read_input_to_pipe(current_redir->filename,
												current_redir->expand_heredoc, data, pipe_fds[1]);
					close(pipe_fds[1]); 
					if (g_received_signal == SIGINT || read_status_code == 130) // Interrupted
					{
						close(pipe_fds[0]); 
						data->last_exit_status = 130;
						set_signal_handlers_prompt(); 
						cleanup_all_heredoc_fds(cmd_list); // Cleanup
						return (EXIT_FAILURE); // Signal interruption
					}
					else if (read_status_code < 0) // Other read/write error
					{
						close(pipe_fds[0]);
						data->last_exit_status = EXIT_FAILURE;
						set_signal_handlers_prompt();
						cleanup_all_heredoc_fds(cmd_list);
						return (EXIT_FAILURE);
					}
					current_redir->heredoc_fd = pipe_fds[0]; // Store read end
				}
			}
			current_redir = current_redir->next;
		}
		if (g_received_signal == SIGINT) break;
		current_cmd = current_cmd->next;
	}
	set_signal_handlers_prompt(); 
	if (g_received_signal == SIGINT)
	{
		data->last_exit_status = 130;
		cleanup_all_heredoc_fds(cmd_list);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
