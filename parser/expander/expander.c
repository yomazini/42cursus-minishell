/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:20:31 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/14 09:50:03 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_should_expand_heredoc_content(const char *raw_delimiter)
{
	size_t	len;

	if (!raw_delimiter)
		return (false);
	len = ft_strlen(raw_delimiter);
	if (len >= 2)
	{	
		if (ft_isquot(raw_delimiter[0]) || ft_isquot(raw_delimiter[len - 1]))
			return (false);
	}
	return (true);	
}
void	ft_expander(t_token **token, t_data *data)
{
	t_token	*current;

	current = *token;
	data->herdoc = false;
	while (current)
	{
		if (current->type == TOKEN_REDIR_HEREDOC
			&& current->next && current->next->type == TOKEN_WORD)
		{
			data->herdoc = true;
			if (ft_should_expand_heredoc_content(current->next->value))
				current->next->exp_in_herdoc = true;
		}
		else if (ft_isexpandable(current))
			ft_expand(&current, data);
		ft_clean_up(&current);
		current = current->next;
	}
}
