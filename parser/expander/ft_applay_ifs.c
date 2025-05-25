/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_applay_ifs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 19:38:12 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/24 15:51:08 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static bool	ft_pre_split(t_token **curr_tkn_ptr)
{
	if (!(*curr_tkn_ptr) || !(*curr_tkn_ptr)->value)
		return (true);
	if (ft_isall_spaces((*curr_tkn_ptr)->value))
	{
		free((*curr_tkn_ptr)->value);
		(*curr_tkn_ptr)->value = ft_strdup("");
		return (true);
	}
	return (false);
}

static int	ft_insert(t_token **tkn_ptr, char **split_res,
					size_t st_idx, size_t n_spl)
{
	t_token	*new_node;
	size_t	i;

	i = st_idx;
	while (i < n_spl && split_res[i])
	{
		new_node = ft_token_new(split_res[i], TOKEN_WORD);
		if (!new_node)
		{
			free(split_res[i]);
			i++;
			while (i < n_spl)
			{
				if (split_res[i])
					free(split_res[i]);
				i++;
			}
			return (0);
		}
		new_node->next = (*tkn_ptr)->next;
		(*tkn_ptr)->next = new_node;
		*tkn_ptr = new_node;
		i++;
	}
	return (1);
}

static void	ft_applay(t_token **curr_tkn_ptr, char **split_res, size_t n_spl)
{
	t_token	*proc_token;

	if (n_spl == 0 || !split_res[0])
	{
		free((*curr_tkn_ptr)->value);
		(*curr_tkn_ptr)->value = ft_strdup("");
		return ;
	}
	free((*curr_tkn_ptr)->value);
	(*curr_tkn_ptr)->value = split_res[0];
	proc_token = *curr_tkn_ptr;
	if (n_spl > 1)
		ft_insert(&proc_token, split_res, 1, n_spl);
	*curr_tkn_ptr = proc_token;
}

void	ft_applay_ifs(t_token **curr_tkn_ptr)
{
	size_t	n_splited;
	char	**split_res;
	size_t	i;

	if (ft_pre_split(curr_tkn_ptr))
		return ;
	n_splited = 0;
	split_res = ft_split_string_by_whitespace((*curr_tkn_ptr)->value,
			&n_splited);
	if (!split_res)
		return ;
	ft_applay(curr_tkn_ptr, split_res, n_splited);
	i = 1;
	while (i < n_splited && split_res[i])
	{
		free(split_res[i]);
		split_res[i] = NULL;
		i++;
	}
	free(split_res);
}
