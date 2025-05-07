/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:27:28 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 15:09:59 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

size_t	ft_count_cmd(t_token *token)
{
	t_token	*current;
	size_t	count_pipe;

	current = token;
	count_pipe = 0;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			count_pipe++;
		current = current->next;
	}
	return (count_pipe + 1);
}

size_t	ft_viclen(char **argv)
{
	size_t	i;

	if (!argv)
		return (0);
	i = 0;
	while (argv[i])
		i++;
	return (i);
}
