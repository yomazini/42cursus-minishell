/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:44:52 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/08 20:37:55 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h" 

// Assume ft_list_getenv, ft_itoa, ft_strdup, ft_strjoin, ft_substr exist

// Helper to check for start of variable name ($ followed by letter or _)
static int	is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

// Helper to check for valid subsequent variable name characters
static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

// Helper function to append a single character to an allocated string
// Reallocates the string. Returns new pointer or NULL on failure.
static char	*append_char(char *str, char c)
{
	size_t	len;// line1
	char	*new_str;
	char	append[2];

	if (!str) // Handle case where initial str might be NULL
		return (ft_strdup(&c)); // Should not happen if str starts with ""

	len = ft_strlen(str);
	new_str = (char *)malloc(sizeof(char) * (len + 2));
	if (!new_str)
	{
		free(str); // Free original on malloc failure
		return (NULL);
	}
	ft_strlcpy(new_str, str, len + 1); // Copy original string
	append[0] = c;
	append[1] = '\0';
	ft_strlcat(new_str, append, len + 2); // Append the new character
	free(str); // Free old string
	return (new_str);
}

// Expands variables ($VAR, $?) in a single line.
// Returns a NEWLY ALLOCATED string with expansions done, or NULL on malloc error.
// Caller MUST free the result.
char	*expand_heredoc_line(char *line, t_data *data)
{
	char	*expanded_line;// L31
	char	*var_name;
	char	*var_value;
	char	*tmp_str;
	int		i;
	int		j;

	if (!line) return (NULL); // Safety check
	expanded_line = ft_strdup(""); // Start with empty string
	if (!expanded_line) return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1]) // Potential expansion start
		{
			i++; // Look at char after '$'
			if (line[i] == '?') // Handle $?
			{
				tmp_str = ft_itoa(data->last_exit_status); // Use Libft itoa
				if (!tmp_str) { free(expanded_line); return (NULL); }
				var_value = ft_strjoin(expanded_line, tmp_str);
				free(expanded_line); free(tmp_str);
				if (!var_value) return (NULL);
				expanded_line = var_value;
				i++; // Move past '?'
			}
			else if (is_var_start(line[i])) // Handle $VAR
			{
				j = i;
				while (line[j] && is_var_char(line[j])) j++;
				var_name = ft_substr(line, i, j - i);
				if (!var_name) { free(expanded_line); return (NULL); }
				var_value = ft_list_getenv(data->env_list, var_name); // Find value
				free(var_name);
				if (var_value) // Append value if found
				{
					tmp_str = ft_strjoin(expanded_line, var_value);
					free(expanded_line);
					if (!tmp_str) return (NULL);
					expanded_line = tmp_str;
				} // If !var_value, append nothing
				i = j; // Move main index past var name
			}
			else // Not $? or $VAR (e.g., "$ ", "$!", "$$") - Append '$' literally
			{
				expanded_line = append_char(expanded_line, '$'); // Append '$'
				if (!expanded_line) return (NULL);
				// Do NOT increment i here, process char after '$' next loop
			}
		}
		else // Regular character (or lone '$' at end), append literally
		{
			expanded_line = append_char(expanded_line, line[i]);
			if (!expanded_line) return (NULL);
			i++;
		}
	}
	return (expanded_line);
}