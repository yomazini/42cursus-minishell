/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:44 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/16 15:56:43 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

size_t	ft_strlen(const char *string)
{
	size_t	string_len;

	string_len = 0;
	while (string[string_len])
		string_len++;
	return (string_len);
}
