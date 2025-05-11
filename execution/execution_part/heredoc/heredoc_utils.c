/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:44:52 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/11 10:02:00 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h" 


static int	is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*append_char(char *str, char c)
{
	size_t	len;// line1
	char	*new_str;
	char	append[2];

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

char	*expand_heredoc_line(char *line, t_data *data)
{
	char	*expanded_line;// L31
	char	*var_name;
	char	*var_value;
	char	*tmp_str;
	int		i;
	int		j;

	if (!line) return (NULL); 
	expanded_line = ft_strdup(""); 
	if (!expanded_line) return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1]) 
		{
			i++; 
			if (line[i] == '?') 
			{
				tmp_str = ft_itoa(data->last_exit_status);
				if (!tmp_str) { free(expanded_line); return (NULL); }
				var_value = ft_strjoin(expanded_line, tmp_str);
				free(expanded_line); free(tmp_str);
				if (!var_value) return (NULL);
				expanded_line = var_value;
				i++; // Move past '?'
			}
			else if (is_var_start(line[i])) 
			{
				j = i;
				while (line[j] && is_var_char(line[j])) j++;
				var_name = ft_substr(line, i, j - i);
				if (!var_name) { free(expanded_line); return (NULL); }
				var_value = ft_list_getenv(data->env_list, var_name);
				free(var_name);
				if (var_value) 
				{
					tmp_str = ft_strjoin(expanded_line, var_value);
					free(expanded_line);
					if (!tmp_str) return (NULL);
					expanded_line = tmp_str;
				}
				i = j;
			}
			else
			{
				expanded_line = append_char(expanded_line, '$'); 
				if (!expanded_line) return (NULL);
			}
		}
		else 
		{
			expanded_line = append_char(expanded_line, line[i]);
			if (!expanded_line) return (NULL);
			i++;
		}
	}
	return (expanded_line);
}