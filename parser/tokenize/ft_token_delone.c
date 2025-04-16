/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_delone.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:18:35 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/16 15:18:36 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void    ft_token_delone(t_token *node)
{
    if (!node)
        return ;
    if (node->value)
    {
        free(node->value);
        node->value = NULL;
    }
    free(node);
}
