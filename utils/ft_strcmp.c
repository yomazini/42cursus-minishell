/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:05:45 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/24 16:28:47 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	ft_strcmp(const char *alpha, const char *bita)
{
	size_t	i;

	i = 0;
	while (alpha[i] != '\0' && bita[i] != '\0' && alpha[i] == bita[i])
		i++;
	if (alpha[i] == bita[i])
	{
		return (0);
	}
	else
	{
		return (1);
	}
}
