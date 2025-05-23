/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_synax_error_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:18:04 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/23 10:51:33 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	ft_print_syntax_error(char *error_message)
{
	printf("\033[1;31m minishell: syntax error:\033[0m \033[1;36m%s\033[0m\n",
		error_message);
}

int	ft_synax_error_free(const char *line)
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
