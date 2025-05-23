/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_should_expand_heredoc_content.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 20:41:13 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/22 21:24:32 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

bool	ft_should_expand_heredoc_content(const char *raw_delimiter)
{
	size_t	len;

	if (!raw_delimiter)
		return (false);
	len = ft_strlen(raw_delimiter);
	if (len >= 2)
	{
		if (ft_isquot(raw_delimiter[0]) || ft_isquot(raw_delimiter[len - 1]))
			return (false);
	}
	return (true);
}
