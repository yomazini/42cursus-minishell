/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_add_back.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:08:57 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 14:09:27 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static t_redir	*ft_redir_last(t_redir *redir)
{
	if (!redir)
		return (NULL);
	while (redir->next)
	{
		redir = redir->next;
	}
	return (redir);
}

void	ft_redir_add_back(t_redir **redir_list, t_redir *new_redir)
{
	t_redir	*last;

	if (!redir_list || !new_redir)
		return ;
	if (*redir_list)
	{
		last = ft_redir_last(*redir_list);
		last->next = new_redir;
	}
	else
		*redir_list = new_redir;
}
