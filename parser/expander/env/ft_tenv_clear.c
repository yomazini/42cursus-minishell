/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tenv_clear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:15:32 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 13:15:33 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../parser.h"

void	ft_tenv_clear(t_env **token_list)
{
	t_env	*current;
	t_env	*next_node;

	if (!token_list || !*token_list)
		return ;
	current = *token_list;
	while (current)
	{
		next_node = current->next;
		ft_tenv_delone(current);
		current = next_node;
	}
	*token_list = NULL;
}
