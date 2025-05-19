/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:08:59 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/19 15:04:52 by ymazini          ###   ########.fr       */
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
	int	i;

	i = 0;
	if (!arg || arg[0] == '\0')
		return (0);
	while (arg[i] == ' ' || (arg[i] >= '\t' && '\r' >= arg[i]))
		i++;
	return (arg[i] == '\0');
}

int	ft_valid_number(char *s)
{
	int		i;
	size_t	len;
	int		digit_count;

	i = 0;
	digit_count = 0;
	if (!s || !s[i])
		return (0);
	s = ft_strtrim(s, " ");
	len = ft_strlen(s);
	if (s[i] == '-' || s[i] == '+')
	{
		i++;
		if (!s[i])
			return (0);
	}
	if (!ft_isdigit(s[i]))
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		digit_count++;
		i++;
	}
	if (digit_count == 0 || (digit_count > 19
			&& (len > 20 || (len == 20 && s[0] != '-')))
		|| (digit_count > 19 && len > 19 && s[0] == '-'))
		return (0);
	return (1);
}
