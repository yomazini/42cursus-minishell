/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:51:48 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/03 09:53:57 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	quote_error(const char *line)
{
	int	index;
	int	in_quote_flag;

	in_quote_flag = 0;
	index = 0;
	while (line[index])
	{
		if (in_quote_flag == 0 && line[index] == '\'')
			in_quote_flag = 1;
		else if (in_quote_flag == 0 && line[index] == '\"')
			in_quote_flag = 2;
		else if (in_quote_flag == 1 && line[index] == '\'')
			in_quote_flag = 0;
		else if (in_quote_flag == 2 && line[index] == '\"')
			in_quote_flag = 0;
		index++;
	}
	return (in_quote_flag != 0);
}

static int	ft_helper_pipe_func(const char *line)
{
	size_t	lenght;
	int		start_index;
	int		end_index;

	lenght = ft_strlen(line);
	if (lenght == 0)
		return (0);
	start_index = 0;
	while (line[start_index] && ft_isspace(line[start_index]))
		start_index++;
	if (line[start_index] == '|')
		return (1);
	end_index = lenght - 1;
	while (end_index > 0 && ft_isspace(line[end_index]))
		end_index--;
	if (line[end_index] == '|')
		return (1);
	return (0);
}

int	pipe_error(const char *line)
{
	int		i;
	bool	last_was_pipe;

	if (ft_helper_pipe_func(line))
		return (1);
	i = 0;
	last_was_pipe = false;
	while (line[i])
	{
		if (ft_isspace(line[i]))
		{
		}
		else if (line[i] == '|')
		{
			if (last_was_pipe)
				return (1);
			last_was_pipe = true;
		}
		else
			last_was_pipe = false;
		i++;
	}
	return (0);
}

int	redir_error(const char *line)
{
	size_t	lenght;
	int		end;

	lenght = ft_strlen(line);
	if (lenght == 0)
		return (0);
	end = lenght - 1;
	while (end >= 0 && ft_isspace(line[end]))
		end--;
	if (end < 0)
		return (0);
	if (line[end] == '>' || line[end] == '<')
		return (1);
	return (0);
}
