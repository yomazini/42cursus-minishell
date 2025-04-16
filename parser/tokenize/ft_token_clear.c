/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_clear.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:18:24 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/16 15:18:26 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void    ft_token_clear(t_token **token_list)
{
    t_token *current;
    t_token *next_node;

    if (!token_list || !*token_list)
        return ;
    current = *token_list;
    while(current)
    {
        next_node = current->next;
        ft_token_delone(current);
        current = next_node;
    }
    *token_list = NULL;
}
