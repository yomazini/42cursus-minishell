/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_synax_error_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:18:04 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/16 15:18:05 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int quote_error(const char *line)
{
	int		index;
	int		in_quote_flag;

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
	return (in_quote_flag != 0); //TODO-> NC (new code or nuclear code)
}
/*	
	if in_quot_flag = 0 -> 0 != 0 false -> no error
	else if in_quot_flag = 1 || 2 != 0 true -> error
	Return 1 (true -> error)
	Return 0 (false -> no error)
	the quastion is is quot_error ?
*/

int ft_helper_pipe_func(const char *line)
{
	size_t lenght;
	int start_index;
	int end_index;

	lenght =  ft_strlen(line);
	if (lenght == 0)
		return (0);
	start_index = 0;
	while(line[start_index] && ft_isspace(line[start_index]))
		start_index++;
	if (!line[start_index] || line[start_index] == '|')
		return (1);
	end_index = lenght - 1;
	while (end_index >= 0 && ft_isspace(line[end_index]))
		end_index--;
	if (line[end_index] == '|')
		return (1);
	return (0);
}
int pipe_error(const char *line)
{
	int i;
	bool last_was_pipe;

	if (ft_helper_pipe_func(line))
		return (1);
	i = 0;
	last_was_pipe = false;
	while (line[i])
	{
		if (ft_isspace(line[i]))
		{
			// it stop the else if and else parts from running when line[i] is a space.
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
	size_t lenght;
	int end;

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
void	ft_print_syntax_error(char *error_message)
{
	printf("\033[1;31msyntax error:\033[0m \033[1;36m%s\033[0m\n", error_message);
}
int ft_synax_error_free(const char *line)
{
	if (!line)
		return (0);
	if (quote_error(line))
    	ft_print_syntax_error("unclosed quote");
	else if (pipe_error(line))
    	ft_print_syntax_error("unexpected token `|`");
	else if (redir_error(line))
    	ft_print_syntax_error("unexpected redirection at end");
	return (!quote_error(line) && !pipe_error(line) && !redir_error(line));
}
