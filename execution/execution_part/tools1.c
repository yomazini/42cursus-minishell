/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:08:59 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/26 11:47:50 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	ft_is_only_whitespace(char *arg)
{
	int i = 0;
	if (!arg || arg[0] == '\0')
		return (0);
	while (arg[i] == ' ' || (arg[i] >= '\t' && '\r' >= arg[i]))
		i++;
	return (arg[i] == '\0');
}

int	ft_valid_number(char *s)
{
	int	i;

	i = 0;
	if (!s || !s[i])
		return (0);
	if (s[i] == '-' || s[i] == '+')
	{
		i++;
		if (!s[i])
			return (0);
	}
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

