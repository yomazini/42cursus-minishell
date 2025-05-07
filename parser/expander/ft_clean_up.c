/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_up.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:26:14 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 11:36:11 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static void	ft_helper_func(char *new_str, t_token **token)
{
	int		i;
	int		j;
	char	alpha_quote;

	i = 0;
	j = 0;
	alpha_quote = '\0';
	while ((*token)->value[i])
	{
		if (!alpha_quote && ft_isquot((*token)->value[i]))
		{
			alpha_quote = (*token)->value[i];
			i++;
		}
		else if (alpha_quote && (*token)->value[i] == alpha_quote)
		{
			i++;
			alpha_quote = '\0';
		}
		else
			new_str[j++] = (*token)->value[i++];
	}
	new_str[j] = '\0';
	free((*token)->value);
	(*token)->value = new_str;
}

void	ft_clean_up(t_token **token)
{
	char	*new_str;
	size_t	len;

	if (!token || !*token || !(*token)->value)
		return ;
	len = ft_strlen((*token)->value);
	new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return ;
	ft_helper_func(new_str, token);
}
