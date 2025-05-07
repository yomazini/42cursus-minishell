/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:20:31 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 11:21:07 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	ft_expander(t_token **token, t_data *data)
{
	t_token	*current;

	current = *token;
	data->herdoc = false;
	while (current)
	{
		if (current->type == TOKEN_REDIR_HEREDOC
			&& current->next && current->next->type == TOKEN_WORD)
			data->herdoc = true;
		else if (ft_isexpandable(current))
			ft_expand(&current, data);
		ft_clean_up(&current);
		current = current->next;
	}
}
