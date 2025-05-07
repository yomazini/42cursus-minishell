/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:39:55 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 15:33:22 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static int	int_len(long c)
{
	int	i;

	i = 0;
	if (c <= 0)
	{
		i++;
		c = -c;
	}
	while (c)
	{
		c /= 10;
		i++;
	}
	return (i);
}

static char	*ft_itoa_ps(int n)
{
	long	c;
	char	*str;
	int		len;

	c = n;
	len = int_len(c);
	str = (char *)malloc (len + 1);
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (c == 0)
		str[0] = '0';
	if (c < 0)
	{
		str[0] = '-';
		c = -c ;
	}
	while (c)
	{
		str[len--] = (c % 10) + '0';
		c /= 10;
	}
	return (str);
}

char	*append_single_char(char *new_str, char c)
{
	char	*temp;
	char	signel_char_string[2];

	signel_char_string[0] = c;
	signel_char_string[1] = '\0';
	temp = ft_strjoined(new_str, signel_char_string);
	free(new_str);
	new_str = temp;
	return (new_str);
}

char	*ft_append_vt(char *new_str, const char *orign, t_env *env, int *i, int peak)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

	var_name = ft_build_variable_name(orign, peak, i);
	if (!var_name)
		return (new_str);
	var_value = ft_isvariablet_exist(env, var_name);
	if (!var_value)
		return (free(var_name), new_str);
	temp = ft_strjoined(new_str, var_value);
	if (!temp)
		return (new_str);
	free(new_str);
	new_str = temp;
	free(var_name);
	return (new_str);
}

char	*ft_append_exit_status(char *new_str, int last_exit_status)
{
	char	*str;
	size_t	len;
	char	*value;
	char	*temp;

	str = ft_itoa_ps(last_exit_status);
	len = ft_strlen(str);
	value = malloc(sizeof(char) * (len + 1));
	ft_strncpy(value, str, (len + 1));
	temp = ft_strjoined(new_str, value);
	free(new_str);
	new_str = temp;
	return (new_str);
}
