/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:14:28 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 13:14:31 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../parser.h"

char	*ft_strdup(const char *string)
{
	size_t	size;
	char	*cpy;
	int		i;

	if (!string)
		return (NULL);
	size = ft_strlen((char *)string);
	cpy = malloc(sizeof(char) * (size + 1));
	if (!cpy)
		return (NULL);
	i = 0;
	while (string[i])
	{
		cpy[i] = string[i];
		i++;
	}
	cpy[size] = '\0';
	return (cpy);
}
