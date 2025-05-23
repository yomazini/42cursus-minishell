/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:20:31 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/22 21:16:39 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_check_pipe_echo(t_token *current, t_data *data)
{
	if (current->type == TOKEN_PIPE && current->next
		&& current->next->value && ft_strcmp(current->next->value, "echo") == 0)
	{
		data->echo_pipe_flag = true;
	}
}

static void	ft_handle_herdoc_expantion_flags(t_token *curr, t_data *data)
{
	data->herdoc = true;
	if (ft_should_expand_heredoc_content(curr->next->value))
		curr->next->exp_in_herdoc = true;
}

static void	ft_process_expandable_token(t_token **curr, t_data *data)
{
	bool	split_needed;

	ft_expand(curr, data);
	if (!*curr)
		return ;
	split_needed = data->field_splitting_needed;
	if ((*curr)->type == TOKEN_WORD && split_needed && (*curr)->value)
	{
		ft_applay_ifs(curr);
		if (!(*curr))
			return ;
	}
	if (split_needed && (*curr)->value
		&& (ft_isall_spaces((*curr)->value) || (*curr)->value[0] == '\0'))
	{
		(*curr)->empty_tkn = true;
	}
}

void	ft_expander(t_token **token_head, t_data *data)
{
	t_token	*curr;

	data->herdoc = false;
	data->echo_pipe_flag = false;
	curr = *token_head;
	while (curr)
	{
		ft_check_pipe_echo(curr, data);
		if (curr->type == TOKEN_REDIR_HEREDOC
			&& curr->next && curr->next->type == TOKEN_WORD)
		{
			ft_handle_herdoc_expantion_flags(curr, data);
		}
		else if (ft_isexpandable(curr))
			ft_process_expandable_token(&curr, data);
		else
			ft_clean_up(&curr);
		if (curr)
			curr = curr->next;
	}
}
