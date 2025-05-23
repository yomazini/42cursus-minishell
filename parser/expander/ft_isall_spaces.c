/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isall_spaces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 20:32:35 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/22 21:23:46 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

bool	ft_isall_spaces(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (!ft_isspace(string[i]))
			return (false);
		i++;
	}
	return (true);
}
