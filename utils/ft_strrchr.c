/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:15:53 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/24 16:28:47 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_strrchr(const char *s, int c)
{
	char	chr;
	int		s_lastindex;

	s_lastindex = ft_strlen(s);
	chr = (char)c;
	if (s[s_lastindex] == chr)
		return ((char *)&s[s_lastindex]);
	s_lastindex -= 1;
	while (0 <= s_lastindex)
	{
		if (s[s_lastindex] == chr)
			return ((char *)&s[s_lastindex]);
		s_lastindex--;
	}
	return (NULL);
}
