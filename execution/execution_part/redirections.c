/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:47:59 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/15 22:09:14 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	handle_heredoc_redir(int heredoc_fd)
{
	if (heredoc_fd < 0)
	{
		ft_putstr_fd("minishell: heredoc error: \
			invalid internal heredoc_fd for dup2\n", 2);
		errno = EBADF;
		return (redir_error01("heredoc"));
	}
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		redir_error01("heredoc (dup2 failed)");
		close(heredoc_fd);
		return (1);
	}
	close(heredoc_fd);
	return (0);
}

int	redir_error01(char *filename)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (filename)
		ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
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

int	apply_redirections(t_cmd *cmd)
{
	int		status;
	t_redir	*current_redir;

	status = EXIT_SUCCESS;
	if (!cmd || !cmd->redir)
		return (0);
	current_redir = cmd->redir;
	while (current_redir)
	{
		if (current_redir->type == TOKEN_REDIR_IN)
			status = handle_infile_redir(current_redir->filename);
		else if (current_redir->type == TOKEN_REDIR_HEREDOC)
			status = handle_heredoc_redir(current_redir->heredoc_fd);
		else if (current_redir->type == TOKEN_REDIR_OUT)
			status = handle_outfile_trunc_n_append__redir(
					current_redir->filename, 0);
		else if (current_redir->type == TOKEN_REDIR_APPEND)
			status = handle_outfile_trunc_n_append__redir(
					current_redir->filename, 1);
		if (status != 0)
			return (EXIT_FAILURE);
		current_redir = current_redir->next;
	}
	return (EXIT_SUCCESS);
}
