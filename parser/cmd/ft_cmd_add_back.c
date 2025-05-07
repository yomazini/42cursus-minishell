/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmd_add_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:18:45 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 13:18:54 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static t_cmd	*ft_cmd_last(t_cmd *cmd)
{
	if (!cmd)
		return (NULL);
	while (cmd->next)
	{
		cmd = cmd->next;
	}
	return (cmd);
}

void	ft_cmd_add_back(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*last;

	if (!cmd_list || !new_cmd)
		return ;
	if (*cmd_list)
	{
		last = ft_cmd_last(*cmd_list);
		last->next = new_cmd;
	}
	else
		*cmd_list = new_cmd;
}
