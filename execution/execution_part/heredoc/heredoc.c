/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:43:01 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/13 15:15:54 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h"

// extern volatile sig_atomic_t	g_received_signal;
extern int g_tmp;

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
		if (g_tmp == 3)
			return (130); // Signal interruption

		line = readline("> "); // Heredoc prompt

		// Check signal flag *after* readline (handler might have run)
		if (g_tmp == 3)
		{
			if (line)
				free(line); // Readline might have allocated something
			return (130); // Signal interruption
		}

		if (!line) // Actual EOF (Ctrl+D)
		{
			if (g_tmp == 3)
				return (130);
			ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
			ft_putstr_fd("end-of-file (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			return (0);
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
	int					saved_stdin_fd = -1; // Variable to hold the duplicated STDIN
	int					return_code = EXIT_SUCCESS; // Assume success initially

	// Save STDIN
	saved_stdin_fd = dup(STDIN_FILENO);
	if (saved_stdin_fd == -1) {
		perror("minishell: dup (saving stdin)");
		data->last_exit_status = EXIT_FAILURE; // Set error status
		return (EXIT_FAILURE);
	}

	set_signal_handlers_heredoc();
	g_tmp = 0; 
	current_cmd = cmd_list;
	while (current_cmd != NULL)
	{
		if (g_tmp == 3) { return_code = EXIT_FAILURE; break; } // Interruption detected
		current_redir = current_cmd->redir;
		while (current_redir != NULL)
		{
			if (g_tmp == 3) { return_code = EXIT_FAILURE; break; } // Interruption detected

			if (current_redir->type == TOKEN_REDIR_HEREDOC)
			{
				if (current_redir->heredoc_fd == -1) 
				{
					if (pipe(pipe_fds) == -1) {
						perror("minishell: heredoc pipe");
						data->last_exit_status = EXIT_FAILURE;
						return_code = EXIT_FAILURE;
						goto cleanup_and_exit; // Use goto for centralized cleanup
					}
					read_status_code = read_input_to_pipe(current_redir->filename,
												current_redir->expand_heredoc, data, pipe_fds[1]);
					close(pipe_fds[1]); 
					if (g_tmp == 3 || read_status_code == 130) // Interrupted
					{
						close(pipe_fds[0]); 
						// data->last_exit_status = 130; // Main loop will handle based on g_tmp=3
						return_code = EXIT_FAILURE;
						goto cleanup_and_exit;
					}
					else if (read_status_code < 0) // Other read/write error
					{
						close(pipe_fds[0]);
						data->last_exit_status = EXIT_FAILURE;
						return_code = EXIT_FAILURE;
						goto cleanup_and_exit;
					}
					current_redir->heredoc_fd = pipe_fds[0];
				}
			}
			if (return_code == EXIT_FAILURE) break; // Break inner loop if outer detected interruption
			current_redir = current_redir->next;
		}
		if (return_code == EXIT_FAILURE) break; // Break outer loop
		current_cmd = current_cmd->next;
	}

cleanup_and_exit: // Label for cleanup
	set_signal_handlers_prompt(); // Always restore prompt handlers

	// Restore STDIN
	if (saved_stdin_fd != -1) {
		if (dup2(saved_stdin_fd, STDIN_FILENO) == -1) {
			perror("minishell: dup2 (restoring stdin)");
			// This is a critical error, shell might be unusable for input
			// but we should still try to close saved_stdin_fd.
		}
		close(saved_stdin_fd);
	}

	if (g_tmp == 3) // If global flag indicates overall interruption
	{
		// data->last_exit_status = 130; // Main loop will handle this
		cleanup_all_heredoc_fds(cmd_list); // Close any opened heredoc FDs
		return (EXIT_FAILURE); // Ensure failure is returned
	}
	if (return_code == EXIT_FAILURE) { // If any other error caused failure path
		cleanup_all_heredoc_fds(cmd_list);
	}
	return (return_code); // EXIT_SUCCESS or EXIT_FAILURE from other errors
}