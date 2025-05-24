/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 22:04:58 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/24 16:28:47 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_strdup(const char *src)
{
	size_t	size;
	char	*str;

	size = ft_strlen(src);
	str = malloc (size + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, src, size);
	str [size] = '\0';
	return (str);
}
