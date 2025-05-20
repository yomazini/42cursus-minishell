/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:36:28 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/20 13:43:00 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h"

int	write_and_free(int fd, char *line_to_write, char *line, bool expand)
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

void	restore_after_heredoc(int saved_stdin_fd)
{
	set_signal_handlers_prompt();
	if (saved_stdin_fd >= 0)
	{
		if (dup2(saved_stdin_fd, STDIN_FILENO) < 0)
			perror("minishell: dup2 (restoring stdin after heredoc)");
		close(saved_stdin_fd);
	}
}

int	handle_delimiter(char *line, char *delimiter, size_t len)
{
	if (ft_strncmp(line, delimiter, len + 1) == 0)
	{
		(free(line), line = NULL);
		return (1);
	}
	return (0);
}

int	process_line(char *line, bool expand, t_data *data,
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
