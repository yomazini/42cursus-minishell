/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:47:59 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/06 21:02:03 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"



// Handles '<< DELIMITER' (uses pre-opened heredoc_fd from pipe's read end)
// int handle_heredoc_redir(int heredoc_fd)
// {
// 	if (heredoc_fd < 0)
// 		return (errno = EBADF, redir_error01("heredoc (invalid fd)"));

// 	// Redirect stdin (FD 0) to the heredoc input FD (pipe's read end)
// 	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
// 	{
// 		close (heredoc_fd); // Close if dup2 fails
// 		return (redir_error01("heredoc (dup2 failed)"));
// 	}
// 	// Close the original heredoc pipe read end fd after successful dup2
// 	close(heredoc_fd);
// 	return (0); // Success
// }

// int handle_heredoc_redir(int heredoc_fd)
// {
// 	if (heredoc_fd < 0)
// 		return (errno = EBADF, redir_error01("heredoc (invalid fd)"));

// 	// *** FIX: Correct parenthesis for dup2 condition ***
// 	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
// 	{
// 		close (heredoc_fd); // Close original if dup2 fails
// 		return (redir_error01("heredoc (dup2 failed)"));
// 	}
// 	// *** END FIX ***

// 	close(heredoc_fd); // Close the original FD after successful dup2
// 	return (0);
// }

// int handle_heredoc_redir(int heredoc_fd)
// {
// 	// Check if FD is valid before dup2
// 	if (heredoc_fd < 0)
// 	{
// 		// This means process_heredocs failed to set it or it was never set
// 		ft_putstr_fd("minishell: heredoc error: invalid internal heredoc_fd\n", 2);
// 		return (1); // Indicate failure
// 	}

// 	// Redirect stdin (FD 0) to the heredoc input FD (pipe's read end)
// 	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
// 	{
// 		// dup2 failed. Print error based on errno set by dup2.
// 		redir_error01("heredoc (dup2 failed)"); // Pass a name for context
// 		close(heredoc_fd); // Still attempt to close the original FD
// 		return (1);        // Indicate failure
// 	}

// 	// Close the original heredoc pipe read end fd after successful dup2
// 	// It's no longer needed directly by this child; FD 0 is the handle now.
// 	close(heredoc_fd);
// 	return (0); // Success
// }

int handle_heredoc_redir(int heredoc_fd)
{
	if (heredoc_fd < 0) // This implies process_heredocs had an issue
	{
		ft_putstr_fd("minishell: heredoc error: invalid internal heredoc_fd for dup2\n", 2);
		errno = EBADF; // Explicitly set errno for clarity if redir_error01 uses it
		return (redir_error01("heredoc"));
	}
	// Redirect stdin (FD 0) to the heredoc input FD (pipe's read end)
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		// dup2 failed. Print error based on errno set by dup2.
		redir_error01("heredoc (dup2 failed)");
		close(heredoc_fd); // Attempt to close the original FD
		return (1);        // Indicate failure
	}
	// Close the original heredoc pipe read end fd after successful dup2
	// It's no longer needed directly by this child; FD 0 is the handle now.
	close(heredoc_fd);
	return (0); // Success
}

int	redir_error01(char *filename)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (filename)
		ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(" :", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (1);
}

int	handle_infile_redir(char *filename)
{
	int	fd;

	if (!filename)
		return (errno = EINVAL, redir_error01("missing infile name"));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (redir_error01(filename));
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (redir_error01(filename));
	}
	close (fd);
	return (0);
}

int	handle_outfile_trunc_n_append__redir(char *filename, int append)
{
	int	fd;

	if (!filename)
		return (errno = EINVAL, redir_error01("missing infile name"));
	if (append)
		fd = open (filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open (filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (redir_error01(filename));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close (fd);
		return (redir_error01(filename));
	}
	close (fd);
	return (0);
}

// int	handle_heredoc_redir(int heredoc_fd)
// {
// 	if (heredoc_fd < 0)
// 		return (errno = EBADF, redir_error01("heredoc"));
// 	if (dup2(heredoc_fd, STDIN_FILENO < 0))
// 	{
// 		close (heredoc_fd);
// 		return (redir_error01("heredoc"));
// 	}
// 	close(heredoc_fd);
// 	return (0);
// }


int	apply_redirections(t_cmd *cmd)
{
	int		status;
	t_redir	*current_redir;

	status = EXIT_SUCCESS;
	if (!cmd || !cmd->redir) // Corrected field name
		return (0);
	current_redir = cmd->redir; // Corrected field name
	while (current_redir)
	{
		if (current_redir->type == TOKEN_REDIR_IN)
			status = handle_infile_redir(current_redir->filename);
		else if (current_redir->type == TOKEN_REDIR_HEREDOC) // Correct logic
			status = handle_heredoc_redir(current_redir->heredoc_fd);
		else if (current_redir->type == TOKEN_REDIR_OUT)
			status = handle_outfile_trunc_n_append__redir(current_redir->filename, 0);
		else if (current_redir->type == TOKEN_REDIR_APPEND)
			status = handle_outfile_trunc_n_append__redir(current_redir->filename, 1);
		if (status != 0)
			return (EXIT_FAILURE);
		current_redir = current_redir->next;
	}
	return (EXIT_SUCCESS);
}


// int	apply_redirections(t_cmd *cmd)
// {
// 	int		status;
// 	t_redir	*current_redir;

// 	status = EXIT_SUCCESS;
// 	if (!cmd || !cmd->redir)
// 		return (0);
// 	current_redir = cmd->redir;
// 	while (current_redir)
// 	{
// 		if (current_redir->type == TOKEN_REDIR_HEREDOC)
// 			status = handle_heredoc_redir(current_redir->heredoc_fd);
// 		if (current_redir->type == TOKEN_REDIR_IN)
// 			status = handle_infile_redir(current_redir->filename);
// 		else if (current_redir->type == TOKEN_REDIR_HEREDOC)
// 			status = handle_heredoc_redir(current_redir->heredoc_fd);
// 		else if (current_redir->type == TOKEN_REDIR_OUT)
// 			status = handle_outfile_trunc_n_append__redir
// 				(current_redir->filename, 0);
// 		else if (current_redir->type == TOKEN_REDIR_APPEND)
// 			status = handle_outfile_trunc_n_append__redir
// 				(current_redir->filename, 1);
// 		if (status != 0)
// 			return (EXIT_FAILURE);
// 		current_redir = current_redir->next;
// 	}
// 	return (EXIT_SUCCESS);
// }

// Make sure apply_redirections calls it
// int	apply_redirections(t_cmd *cmd)
// {
// 	int status = EXIT_SUCCESS;
// 	t_redir *current_redir;

// 	if (!cmd || !cmd->redir)
// 		return (0);
// 	current_redir = cmd->redir;
// 	while(current_redir != NULL)
// 	{
// 		// ... other cases ...
// 		if (current_redir->type == TOKEN_REDIR_HEREDOC)
// 			status = handle_heredoc_redir(current_redir->heredoc_fd);
// 		// ... other cases ...

// 		if (status != 0)
// 			return (EXIT_FAILURE); // Exit on first failure
// 		current_redir = current_redir->next;
// 	}
// 	return (EXIT_SUCCESS);
// }
