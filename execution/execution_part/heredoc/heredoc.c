/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:43:01 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/10 15:47:33 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h"

/**
 * @brief Iterates through command list, processes all heredocs.
 *        Creates a pipe for each heredoc, reads input, stores readable FD.
 *
 * @param cmd_list Head of command list.
 * @param data Shell data structure.
 * @return int EXIT_SUCCESS or EXIT_FAILURE.
 */


/*		---   */

// extern volatile sig_atomic_t	g_received_signal;
// char							*expand_heredoc_line(char *line, t_data *data);
// void							heredoc_sigint_handler(int signum);
// void							main_prompt_sigint_handler(int signum); // For restoring
// void							setup_signal_action(int signum, void (*handler)(int), int flags);


// static int	read_input_to_pipe(char *delimiter, bool expand,
// 								t_data *data, int pipe_write_fd)
// {
// 	char	*line;
// 	char	*line_to_write;
// 	size_t	delim_len;

// 	delim_len = ft_strlen(delimiter);
// 	while (1)
// 	{
// 		if (g_received_signal == SIGINT) // Check global flag
// 			return (130); // Indicate interruption
// 		line = readline("> ");
// 		if (g_received_signal == SIGINT) // Check again after readline
// 		{
// 			free(line); // Readline might have allocated
// 			return (130);
// 		}
// 		if (!line) { /* EOF warning */ return (0); } // Normal EOF is not an error
// 		if (ft_strncmp(line, delimiter, delim_len + 1) == 0) { free(line); break; }
// 		if (expand) { 
// 			line_to_write = expand_heredoc_line(line, data);
// 			free(line); // Free original readline buffer
// 			if (!line_to_write) return (-1); // Malloc error in expander
// 		} else { line_to_write = line; }
// 		if (write(pipe_write_fd, line_to_write, ft_strlen(line_to_write)) < 0
// 			|| write(pipe_write_fd, "\n", 1) < 0) 
// 			{
// 				perror("minishell: heredoc: write to pipe failed");

// 				 /* handle error, free, return -1 */ }
// 		if (expand) free(line_to_write);
// 	}
// 	return (0); // Success
// }

// int	process_heredocs(t_cmd *cmd_list, t_data *data)
// {
// 	t_cmd				*current_cmd;
// 	t_redir				*current_redir;
// 	int					pipe_fds[2];
// 	int					read_status;
// 	struct sigaction	sa_old_main_int; // To store main prompt SIGINT handler

// 	// 1. Save current main SIGINT handler & set heredoc SIGINT handler
// 	sigaction(SIGINT, NULL, &sa_old_main_int); // Save current SIGINT
// 	setup_signal_action(SIGINT, heredoc_sigint_handler, 0); // Set heredoc one, NO SA_RESTART

// 	g_received_signal = 0; // Ensure flag is clear before starting
// 	current_cmd = cmd_list;
// 	while (current_cmd != NULL && g_received_signal != SIGINT) {
// 		current_redir = current_cmd->redir; // Use correct field name
// 		while (current_redir != NULL && g_received_signal != SIGINT) {
// 			if (current_redir->type == TOKEN_REDIR_HEREDOC) {
// 				if (current_redir->heredoc_fd == -1) {
// 					if (pipe(pipe_fds) == -1) {
// 						perror("minishell: heredoc: pipe creation failed");
// 						return (data->last_exit_status = EXIT_FAILURE, EXIT_FAILURE);
// 						 /* perror, restore main sigint, return fail */ }
// 					read_status = read_input_to_pipe(current_redir->filename,
// 												current_redir->expand_heredoc, data, pipe_fds[1]);
// 					close(pipe_fds[1]); // Close write end
// 					if (read_status == 130 || g_received_signal == SIGINT) { // Interrupted
// 						close(pipe_fds[0]); // Close read end on interrupt
// 						data->last_exit_status = 130;
// 						sigaction(SIGINT, &sa_old_main_int, NULL); // Restore main SIGINT
// 						return (EXIT_FAILURE);
// 					} else if (read_status < 0) { // Other error
// 						close(pipe_fds[0]);
// 						data->last_exit_status = EXIT_FAILURE;
// 						sigaction(SIGINT, &sa_old_main_int, NULL);
// 						return (EXIT_FAILURE);
// 					}
// 					current_redir->heredoc_fd = pipe_fds[0];
// 				}
// 			}
// 			current_redir = current_redir->next;
// 		}
// 		current_cmd = current_cmd->next;
// 	}
// 	// 2. Restore original main SIGINT handler
// 	sigaction(SIGINT, &sa_old_main_int, NULL);
// 	if (g_received_signal == SIGINT) { // If loop exited due to signal
// 		data->last_exit_status = 130;
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }


/*		----- V2.0  */

// extern volatile sig_atomic_t	g_received_signal;
// char							*expand_heredoc_line(char *line, t_data *data);
// // Prototypes for signal setup functions defined in main.c or signals.c
// void							set_signal_handlers_heredoc(void);
// void							set_signal_handlers_prompt(void);


// static int	read_input_to_pipe(char *delimiter, bool expand,
// 								t_data *data, int pipe_write_fd)
// {
// 	char	*line;
// 	char	*line_to_write;
// 	size_t	delim_len;

// 	delim_len = ft_strlen(delimiter);
// 	while (1)
// 	{
// 		// Global flag is checked *before* readline in process_heredocs' loop
// 		// and after readline returns in process_heredocs
// 		line = readline("> ");

// 		if (g_received_signal == SIGINT) // If our handler was called & set flag
// 		{
// 			free(line); // Readline might return an allocated buffer on interruption
// 			return (130); // Indicate SIGINT interruption
// 		}
// 		if (!line) // EOF (Ctrl+D)
// 		{
// 			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
// 			ft_putstr_fd(delimiter, 2);
// 			ft_putstr_fd("')\n", 2);
// 			return (0); // EOF is not an error for process_heredocs itself, but means done
// 		}
// 		if (ft_strncmp(line, delimiter, delim_len + 1) == 0)
// 		{
// 			free(line);
// 			break ; // Delimiter found
// 		}
// 		if (expand) { 
// 			line_to_write = expand_heredoc_line(line, data);
// 			free(line); // Free original readline buffer
// 			if (!line_to_write) return (-1); // Malloc error in expander
// 			/* ... expansion logic, handle line_to_write, free(line) ... */ }
// 		else { line_to_write = line; }

// 		if (write(pipe_write_fd, line_to_write, ft_strlen(line_to_write)) < 0
// 			|| write(pipe_write_fd, "\n", 1) < 0) {
// 			perror("minishell: heredoc: write to pipe failed");
// 			if (expand) free(line_to_write); else if (line != line_to_write) free(line);
// 			return (-1); // Critical write error
// 		}
// 		if (expand) free(line_to_write); else if (line != line_to_write) free(line);
// 		// If not expand, 'line' will be freed on next readline or if loop breaks
// 	}
// 	return (0); // Success
// }

// int	process_heredocs(t_cmd *cmd_list, t_data *data)
// {
// 	t_cmd	*current_cmd;
// 	t_redir	*current_redir;
// 	int		pipe_fds[2];
// 	int		read_status;

// 	set_signal_handlers_heredoc(); // Set SIGINT to heredoc-specific handler
// 	g_received_signal = 0;         // Clear flag before starting

// 	current_cmd = cmd_list;
// 	while (current_cmd != NULL && g_received_signal != SIGINT) {
// 		current_redir = current_cmd->redir; // Corrected field name
// 		while (current_redir != NULL && g_received_signal != SIGINT) {
// 			if (current_redir->type == TOKEN_REDIR_HEREDOC) {
// 				if (current_redir->heredoc_fd == -1) {
// 					if (pipe(pipe_fds) == -1) { 
// 						perror("minishell: heredoc: pipe creation failed");
// 						return (data->last_exit_status = EXIT_FAILURE, EXIT_FAILURE);
// 						/* perror, restore prompt signals, return fail */ }
// 					read_status = read_input_to_pipe(current_redir->filename,
// 												current_redir->expand_heredoc, data, pipe_fds[1]);
// 					close(pipe_fds[1]); // Always close write end
// 					if (read_status == 130 || g_received_signal == SIGINT) { // Interrupted by SIGINT
// 						close(pipe_fds[0]);
// 						data->last_exit_status = 130;
// 						set_signal_handlers_prompt(); // Restore prompt handlers
// 						return (EXIT_FAILURE);
// 					} else if (read_status < 0) { // Other read/write error
// 						close(pipe_fds[0]);
// 						data->last_exit_status = EXIT_FAILURE;
// 						set_signal_handlers_prompt();
// 						return (EXIT_FAILURE);
// 					}
// 					// Success or EOF (Ctrl+D), store read end
// 					current_redir->heredoc_fd = pipe_fds[0];
// 				}
// 			}
// 			current_redir = current_redir->next;
// 		}
// 		current_cmd = current_cmd->next;
// 	}
// 	set_signal_handlers_prompt(); // Restore prompt handlers
// 	if (g_received_signal == SIGINT) { // If main loop exited due to signal
// 		data->last_exit_status = 130;
// 		// g_received_signal will be reset at start of main loop
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }



extern volatile sig_atomic_t	g_received_signal;
char							*expand_heredoc_line(char *line, t_data *data);
void							set_signal_handlers_prompt(void); // For restoring

static int	read_input_to_pipe(char *delimiter, bool expand,
								t_data *data, int pipe_write_fd)
{
	char	*line;
	char	*line_to_write;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		// Check signal flag *before* readline
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

// Helper to clean up ALL heredoc FDs in a command list
// Useful if processing is aborted midway.
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
	int					read_status_code; // Store return from read_input_to_pipe

	// IMPORTANT: Set heredoc-specific signal handlers
	set_signal_handlers_heredoc();
	g_received_signal = 0; // Clear any pending signal flag before starting

	current_cmd = cmd_list;
	while (current_cmd != NULL)
	{
		if (g_received_signal == SIGINT) break; // Check before processing next cmd
		current_redir = current_cmd->redir;
		while (current_redir != NULL)
		{
			if (g_received_signal == SIGINT) break; // Check before processing next redir
			if (current_redir->type == TOKEN_REDIR_HEREDOC)
			{
				if (current_redir->heredoc_fd == -1) // Process only if not done
				{
					if (pipe(pipe_fds) == -1) {
						perror("minishell: heredoc pipe");
						data->last_exit_status = EXIT_FAILURE;
						set_signal_handlers_prompt(); // Restore before exit
						cleanup_all_heredoc_fds(cmd_list); // Cleanup any opened FDs
						return (EXIT_FAILURE);
					}
					read_status_code = read_input_to_pipe(current_redir->filename,
												current_redir->expand_heredoc, data, pipe_fds[1]);
					close(pipe_fds[1]); // Parent closes write end immediately

					if (g_received_signal == SIGINT || read_status_code == 130) // Interrupted
					{
						close(pipe_fds[0]); // Close the read end as well
						data->last_exit_status = 130;
						set_signal_handlers_prompt(); // Restore prompt handlers
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
					// Success or normal EOF for this heredoc
					current_redir->heredoc_fd = pipe_fds[0]; // Store read end
				}
			}
			current_redir = current_redir->next;
		}
		if (g_received_signal == SIGINT) break;
		current_cmd = current_cmd->next;
	}
	set_signal_handlers_prompt(); // Always restore prompt handlers
	if (g_received_signal == SIGINT) // If main loop exited due to signal
	{
		data->last_exit_status = 130;
		// g_received_signal is reset at start of main loop
		cleanup_all_heredoc_fds(cmd_list); // Ensure cleanup
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS); // All heredocs (if any) processed successfully
}
