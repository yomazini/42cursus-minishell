/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:43:01 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/05 21:48:02 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h"

// Needs prototype for the expander function
char	*expand_heredoc_line(char *line, t_data *data);

/**
 * @brief Reads heredoc input until delimiter, potentially expands lines,
 *        and writes the result to the write end of a pipe.
 *
 * @param delimiter The delimiter string (literal, not expanded).
 * @param expand Boolean flag indicating if expansion should occur.
 * @param data Shell data structure (for expansion context).
 * @param pipe_write_fd File descriptor for the write end of the pipe.
 * @return int 0 on success, 1 on EOF warning, -1 on other error.
 */
static int	read_and_write_heredoc(char *delimiter, bool expand,
								t_data *data, int pipe_write_fd)
{
	char	*line;
	char	*line_to_write;
	size_t	delim_len;
	int		ret_status;

	delim_len = ft_strlen(delimiter);
	ret_status = 0; // Assume success
	while (1)
	{
		line = readline("> "); // Use standard heredoc prompt
		if (!line) // EOF (Ctrl+D) detected
		{
			ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
			ft_putstr_fd("end-of-file (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			ret_status = 1; // Signal EOF was reached
			break;
		}
		// Compare line with the delimiter
		if (ft_strncmp(line, delimiter, delim_len + 1) == 0)
		{
			free(line); // Found delimiter, free line and exit loop
			break;
		}
		// Expand line if required
		if (expand)
		{
			line_to_write = expand_heredoc_line(line, data);
			free(line); // Free original readline buffer
			if (!line_to_write) { ret_status = -1; break; } // Malloc error
		}
		else
			line_to_write = line; // Use original line if no expansion
		// Write line + newline to pipe
		if (write(pipe_write_fd, line_to_write, ft_strlen(line_to_write)) < 0 ||
		    write(pipe_write_fd, "\n", 1) < 0) {
			perror("minishell: heredoc: write to pipe failed");
			ret_status = -1; // Indicate write error
		}
		if (expand) free(line_to_write); // Free if expander allocated it
		// else line_to_write points to line, which is freed if loop continues or already freed if break
		if (ret_status == -1) break; // Exit loop on write error
	} // End while(1)
	return (ret_status);
}

/**
 * @brief Iterates through command list, processes all heredocs.
 *        Creates a pipe for each heredoc, reads input, stores readable FD.
 *
 * @param cmd_list Head of command list.
 * @param data Shell data structure.
 * @return int EXIT_SUCCESS or EXIT_FAILURE.
 */
int	process_heredocs(t_cmd *cmd_list, t_data *data)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;
	int		pipe_fds[2];
	int		read_status;

	// --- TODO: Signal Handling Setup ---
	current_cmd = cmd_list;
	while (current_cmd != NULL /* && check signal flag */)
	{
		current_redir = current_cmd->redir;
		while (current_redir != NULL /* && check signal flag */)
		{
			if (current_redir->type == TOKEN_REDIR_HEREDOC)
			{
				// Process only if FD is not already set (or if it's invalid?)
				if (current_redir->heredoc_fd == -1)
				{
					if (pipe(pipe_fds) == -1) {
						perror("minishell: heredoc pipe");
						return (data->last_exit_status = EXIT_FAILURE, EXIT_FAILURE);
					}
					// Read input into the write end of the pipe
					read_status = read_and_write_heredoc(current_redir->filename,
						current_redir->expand_heredoc, data, pipe_fds[1]);
					// Close the write end - crucial for reader to get EOF
					close(pipe_fds[1]);
					if (read_status < 0 /* || check signal flag */) {
						close(pipe_fds[0]); // Close read end on error too
						// TODO: Cleanup previously opened FDs
						data->last_exit_status = EXIT_FAILURE; // Or 130 for signal
						// TODO: Restore signals
						return (EXIT_FAILURE);
					}
					// Store the READ end FD for the command to use
					current_redir->heredoc_fd = pipe_fds[0];
				}
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	// --- TODO: Restore signals ---
	return (EXIT_SUCCESS);
}
