/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:38:06 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 11:39:40 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	ft_isexpandable(t_token *current)
{
	if (current->type == TOKEN_WORD && (ft_strchr(current->value, '$')))
		return (1);
	return (0);
}

int	ft_isbign_variable(char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c == '_'));
}

int	ft_ispt_variable(char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9')
		|| c == '_');
}

int	ft_peakahead(char c)
{
	if (ft_isspace(c))
		return (0);
	else if (ft_isbign_variable(c))
		return (1);
	else if (c >= '0' && c <= '9')
		return (2);
	else if (c == '?')
		return (3);
	else if (c == '$')
		return (4);
	return (-1);
}
