/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:44:52 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/20 13:10:26 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h" 

static	int	ft_chekc(char c, int start)
{
	if (start)
		return (ft_isalpha(c) || c == '_');
	else
		return (ft_isalnum(c) || c == '_');
}

static char	*append_char(char *str, char c)
{
	size_t	len;
	char	append[2];
	char	*new_str;

	if (!str)
		return (ft_strdup(&c));
	len = ft_strlen(str);
	new_str = (char *)malloc(sizeof(char) * (len + 2));
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	ft_strlcpy(new_str, str, len + 1);
	append[0] = c;
	append[1] = '\0';
	ft_strlcat(new_str, append, len + 2);
	free(str);
	return (new_str);
}

static char	*append_exit_status(char *current_expanded, t_data *data)
{
	char	*status_str;
	char	*temp_join;

	status_str = ft_itoa(data->last_exit_status);
	if (!status_str)
	{
		free(current_expanded);
		return (NULL);
	}
	temp_join = ft_strjoin(current_expanded, status_str);
	free(current_expanded);
	free(status_str);
	return (temp_join);
}

static char	*append_variable_value(char *current_expanded, char *line_segment,
									t_data *data, int *line_idx_ptr)
{
	char	*var_name;
	char	*var_value;
	char	*temp_join;
	int		j;

	j = *line_idx_ptr;
	while (line_segment[j] && ft_chekc(line_segment[j], 0))
		j++;
	var_name = ft_substr(line_segment, *line_idx_ptr, j - *line_idx_ptr);
	if (!var_name)
		return (free(current_expanded), NULL);
	var_value = ft_list_getenv(data->env_list, var_name);
	free(var_name);
	if (var_value)
	{
		temp_join = ft_strjoin(current_expanded, var_value);
		free(current_expanded);
		if (!temp_join)
			return (NULL);
		current_expanded = temp_join;
	}
	*line_idx_ptr = j;
	return (current_expanded);
}

char	*expand_heredoc_line(char *line, t_data *data)
{
	char	*expnd_st;
	int		i;

	expnd_st = ft_strdup("");
	if (!expnd_st)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			i++;
			if (line[i] == '?' && ++i)
				expnd_st = append_exit_status(expnd_st, data);
			else if (ft_chekc(line[i], 1))
				expnd_st = append_variable_value(expnd_st, line, data, &i);
			else
				expnd_st = append_char(expnd_st, '$');
		}
		else
			expnd_st = append_char(expnd_st, line[i++]);
		if (!expnd_st)
			return (NULL);
	}
	return (expnd_st);
}
