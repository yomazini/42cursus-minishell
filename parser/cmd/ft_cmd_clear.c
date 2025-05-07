/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmd_clear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:19:47 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 13:20:14 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	free_arr(char **arr);

static void	ft_cmd_delone(t_cmd *node)
{
	if (!node)
		return ;
	if (node->argv)
	{
		free_arr(node->argv);
		node->argv = NULL;
	}
	if (node->redir)
		ft_redir_clear(&node->redir);
	free(node);
}

void	ft_cmd_clear(t_cmd **cmd_list)
{
	t_cmd	*current;
	t_cmd	*next_node;

	if (!cmd_list || !*cmd_list)
		return ;
	current = *cmd_list;
	while (current)
	{
		next_node = current->next;
		ft_cmd_delone(current);
		current = next_node;
	}
	*cmd_list = NULL;
}
