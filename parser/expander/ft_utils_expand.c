/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:39:55 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/09 15:25:44 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*ft_append_vt(char *new_str, const char *orign, t_data *data, int *i)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

	var_name = ft_build_variable_name(orign, data->peak, i);
	if (!var_name)
		return (new_str);
	var_value = ft_isvariablet_exist(data->env_list, var_name);
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

	str = ft_itoa(last_exit_status);
	len = ft_strlen(str);
	value = malloc(sizeof(char) * (len + 1));
	ft_strncpy(value, str, (len + 1));
	temp = ft_strjoined(new_str, value);
	free(new_str);
	new_str = temp;
	return (new_str);
}
