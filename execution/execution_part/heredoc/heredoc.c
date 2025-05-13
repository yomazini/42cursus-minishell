/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:43:01 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/13 23:24:05 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h"

extern int g_tmp;

static int	read_input_to_pipe(char *delimiter, bool expand,
								t_data *data, int pipe_write_fd)
{
	char	*line;
	char	*line_to_write;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		if (g_tmp == 3) return (130); // Signal interruption
		line = readline("> ");
		if (g_tmp == 3) { if (line) free(line); return (130); }
		if (!line) { /* EOF warning */ return (0); } // Bash status 0 for this case
		if (ft_strncmp(line, delimiter, delim_len + 1) == 0) { free(line); break; }
		if (expand) {
			line_to_write = expand_heredoc_line(line, data);
			free(line);
			if (!line_to_write) return (-1);
		} else {
			line_to_write = line;
		}
		if (write(pipe_write_fd, line_to_write, ft_strlen(line_to_write)) < 0
			|| write(pipe_write_fd, "\n", 1) < 0) {
			perror("minishell: heredoc: write to pipe failed");
			if (expand && line_to_write != line) free(line_to_write);
			else if (!expand) free(line); // Free original if not expanded and write failed
			return (-1);
		}
		if (expand && line_to_write != line) free(line_to_write);
		else if (!expand) free(line); // Free original if not expanded
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
		current_redir = current_cmd->redir; // Assuming 'redir' is the correct field name
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


// Counts only heredocs in the current command list
static int	count_total_heredocs(t_cmd *cmd_list)
{
	t_cmd	*current_cmd = cmd_list;
	t_redir	*current_redir;
	int		count = 0;

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


int	process_heredocs(t_cmd *cmd_list, t_data *data)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;
	int		pipe_fds[2];
	int		read_status_code;
	int		saved_stdin_fd; // To restore original stdin if needed
	int		overall_status;

	// --- 1. Count total heredocs in this command line FIRST ---
	if (count_total_heredocs(cmd_list) > MAX_HEREDOCS)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", STDERR_FILENO);
		data->last_exit_status = 2; // Bash uses 2 for this parsing error
		// TODO: Here later must exit(2) like bash and before it i HAVE TO clean fd and clean all before exit the allocated and clear history and others as well;
		// add those or return to main function 
		// clear_history();
		// ft_cmd_clear(cmd_list);
		exit (2); // Abort before any processing
		//TODO: Check out my Discord minishell
		// return (2); // Abort before any processing
	}

	// --- Initialization and Signal Setup ---
	saved_stdin_fd = -1; // Initialize
	overall_status = EXIT_SUCCESS;
	g_tmp = 0; // Reset global signal flag
	set_signal_handlers_heredoc(); // Set Ctrl+C handler for heredoc input

	// --- Save original STDIN ---
	saved_stdin_fd = dup(STDIN_FILENO);
	if (saved_stdin_fd == -1) {
		perror("minishell: dup (saving stdin for heredoc)");
		set_signal_handlers_prompt(); // Restore original signals
		return (data->last_exit_status = EXIT_FAILURE, EXIT_FAILURE);
	}
	// --- Loop through commands and their redirections ---
	current_cmd = cmd_list;
	while (current_cmd != NULL && overall_status == EXIT_SUCCESS && g_tmp != 3)
	{
		current_redir = current_cmd->redir;
		while (current_redir != NULL && overall_status == EXIT_SUCCESS && g_tmp != 3)
		{
			if (current_redir->type == TOKEN_REDIR_HEREDOC)
			{
				if (current_redir->heredoc_fd == -1) // Process only if not already done
				{
					if (pipe(pipe_fds) == -1) {
						perror("minishell: heredoc pipe");
						overall_status = EXIT_FAILURE; break;
					}
					read_status_code = read_input_to_pipe(current_redir->filename,
												current_redir->expand_heredoc, data, pipe_fds[1]);
					close(pipe_fds[1]); // Close write end immediately
					if (g_tmp == 3 || read_status_code == 130) { // Interrupted
						close(pipe_fds[0]); overall_status = EXIT_FAILURE; break;
					}
					else if (read_status_code < 0) { // Other read/write error
						close(pipe_fds[0]); overall_status = EXIT_FAILURE; break;
					}
					current_redir->heredoc_fd = pipe_fds[0]; // Store read end
				}
			}
			current_redir = current_redir->next;
		}
		if (overall_status == EXIT_FAILURE) break; // Propagate error/interrupt
		current_cmd = current_cmd->next;
	}

	// --- Cleanup and Restore ---
	set_signal_handlers_prompt(); // Restore main prompt signal handlers
	if (saved_stdin_fd != -1) { // Restore STDIN
		if (dup2(saved_stdin_fd, STDIN_FILENO) == -1)
			perror("minishell: dup2 (restoring stdin after heredoc)"); // Non-fatal for now
		close(saved_stdin_fd);
	}

	// --- Final Status Determination ---
	if (g_tmp == 3) { // Interrupted by Ctrl+C during heredoc
		data->last_exit_status = 130;
		cleanup_all_heredoc_fds(cmd_list); // Close any FDs we did open
		return (EXIT_FAILURE);
	}
	if (overall_status == EXIT_FAILURE) { // Some other error occurred
		data->last_exit_status = EXIT_FAILURE; // Ensure status reflects it
		cleanup_all_heredoc_fds(cmd_list);
	}
	return (overall_status); // EXIT_SUCCESS or EXIT_FAILURE from non-signal errors
}
