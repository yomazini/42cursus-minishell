/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:08:59 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/17 21:25:15 by ymazini          ###   ########.fr       */
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

// int	ft_valid_number(char *s)
// {
// 	int	i;

// 	i = 0;
// 	if (!s || !s[i])
// 		return (0);
// 	s = ft_strtrim(s, " ");
// 	if (s[i] == '-' || s[i] == '+')
// 	{
// 		i++;
// 		if (!s[i])
// 			return (0);
// 	}
// 	while (s[i])
// 	{
// 		if (!ft_isdigit(s[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }


// In exit_built_in.c or tools1.c

// Checks if string 's' represents a valid number for exit status.
// Handles optional sign and checks if all other chars are digits.
// Adds a basic length check to catch obviously out-of-range numbers.
int	ft_valid_number(char *s)
{
	int		i;
	size_t	len;
	int		digit_count;

	i = 0;
	digit_count = 0;
	if (!s || !s[i]) // Handle NULL or empty string
		return (0);
	len = ft_strlen(s);
	if (s[i] == '-' || s[i] == '+')
	{
		i++;
		if (!s[i]) // String was only "+" or "-"
			return (0);
	}
	// After sign, check if there are any digits at all
	if (!ft_isdigit(s[i]))
		return (0);

	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0); // Found a non-digit character
		digit_count++;
		i++;
	}
	// If only sign was present but no digits after it, it's invalid
	if (digit_count == 0)
		return (0);
	// Basic overflow check: long long is roughly 19-20 digits.
	// Bash errors out for numbers larger than long long max/min.
	// This is a heuristic. ft_atoi might still truncate/wrap.
	if (digit_count > 19 && (len > 20 || (len == 20 && s[0] != '-'))) // Rough check
		return (0); // Likely out of long long range
	if (digit_count > 19 && len > 19 && s[0] == '-') // Rough check for negative
		return (0);

	return (1); // All characters after optional sign were digits, and length is plausible
}

