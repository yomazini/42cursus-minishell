/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:20:31 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/17 18:14:47 by eel-garo         ###   ########.fr       */
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
void	ft_free_helper(char **vic, size_t i, size_t n_splited)
{
	while (i < n_splited)
	{
		if (vic[i])
			free(vic[i]);
		i++;
	}
	free(vic);
	return ;	
}
bool	ft_isall_spaces(char *string)
{
	int i;

	i = 0;
	while (string[i])
	{
		if (!ft_isspace(string[i]))
			return (false);
		i++;
	}
	return (true);
}
void	ft_applay_ifs(t_token **curr_token)
{
	size_t	n_splited;
	char	**split_value;
	t_token	*insert_point;
	t_token	*new_node;
	size_t	i;
	
	if (!(*curr_token) || !(*curr_token)->value)
		return ;
	if (ft_isall_spaces((*curr_token)->value))
	{
		free((*curr_token)->value);
		(*curr_token)->value = ft_strdup("");
		return ;
	}
	n_splited = 0;
	split_value = ft_split_string_by_whitespace((*curr_token)->value, &n_splited);
	if (split_value == NULL)
		return ;
	if (n_splited == 0)
		return (free(split_value));
	free((*curr_token)->value);
	(*curr_token)->value = split_value[0];
	insert_point = *curr_token;
	i = 1;
	while (i < n_splited && split_value[i])
	{
		new_node = ft_token_new(split_value[i], TOKEN_WORD);
		if (!new_node)
			return (ft_free_helper(split_value, i, n_splited));
		new_node->next = insert_point->next;
		insert_point->next = new_node;
		insert_point = new_node;
		i++;
	}
	*curr_token = insert_point;
	free(split_value);
}
void	ft_expander(t_token **token, t_data *data)
{
	t_token	*current;
	bool	need_cleanup;

	current = *token;
	data->herdoc = false;
	while (current)
	{
		need_cleanup = true;
		if (current->type == TOKEN_REDIR_HEREDOC
			&& current->next && current->next->type == TOKEN_WORD)
		{
			data->herdoc = true;
			if (ft_should_expand_heredoc_content(current->next->value))
				current->next->exp_in_herdoc = true;
			need_cleanup = false;
		}
		else if (ft_isexpandable(current))
		{
			ft_expand(&current, data);
			if (current->type == TOKEN_WORD && data->field_splitting_needed
				&& current->value)
			{
				ft_applay_ifs(&current);
			}
			if (data->field_splitting_needed && 
				(ft_isall_spaces(current->value) || current->value[0] == '\0'))
				{
					current->empty_tkn = true;
				}
			need_cleanup = false;
		}
		if (need_cleanup)
			ft_clean_up(&current);
		current = current->next;
	}
}
