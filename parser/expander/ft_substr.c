/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:06:50 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 17:57:04 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	char	*ptr;
// 	char	*substr;
// 	size_t	i;
// 	size_t	s_len;

// 	if (!s)
// 		return (NULL);
// 	s_len = ft_strlen(s);
// 	if (start >= s_len)
// 		return (ft_strdup(""));
// 	if ((start + len) > s_len)
// 		len = s_len - start;
// 	ptr = (char *)s;
// 	substr = (char *)malloc((len * sizeof(char)) + 1);
// 	if (!substr)
// 		return (NULL);
// 	i = 0;
// 	while (i < len && s[i])
// 	{
// 		substr[i] = ptr[start];
// 		i++;
// 		start++;
// 	}
// 	substr[i] = '\0';
// 	return (substr);
// }
