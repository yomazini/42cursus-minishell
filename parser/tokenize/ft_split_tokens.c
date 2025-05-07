/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:32 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 16:38:28 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static size_t	ft_get_token_len(const char *line, int *k)
{
	int		start;
	char	quote;

	start = *k;
	if (ft_isoperater(line[*k]))
	{
		*k += 1 + ft_isdouble_op(line, *k);
		return (*k - start);
	}
	while (line[*k])
	{
		if (ft_isquot(line[*k]))
		{
			quote = line[(*k)++];
			while (line[*k] && line[*k] != quote)
				(*k)++;
			if (line[*k] == quote)
				(*k)++;
		}
		else if (ft_isoperater(line[*k]) || ft_isspace(line[*k]))
			break ;
		else
			(*k)++;
	}
	return (*k - start);
}

static size_t	ft_count_tokens_revised(const char *line)
{
	int		k;
	size_t	cnt_tokens;
	size_t	len;

	k = 0;
	cnt_tokens = 0;
	while (line[k])
	{
		while (line[k] && ft_isspace(line[k]))
			k++;
		if (line[k])
		{
			len = ft_get_token_len(line, &k);
			if (len > 0)
				cnt_tokens++;
		}
	}
	return (cnt_tokens);
}

char	*ft_alloc_token(int token_len, int start_index, const char *line)
{
	char	*token_str;

	if (token_len <= 0)
		return (ft_strdup(""));
	token_str = malloc(sizeof(char) * (token_len + 1));
	if (!token_str)
		return (NULL);
	ft_strncpy(token_str, &line[start_index], token_len);
	token_str[token_len] = '\0';
	return (token_str);
}

static char	**filltkn_array_revised(char **token_array, const char *line)
{
	int		i;
	int		k;
	int		token_start;
	size_t	token_len;

	i = 0;
	k = 0;
	while (line[k])
	{
		while (line[k] && ft_isspace(line[k]))
			k++;
		if (line[k])
		{
			token_start = k;
			token_len = ft_get_token_len(line, &k);
			if (token_len > 0)
			{
				token_array[i] = ft_alloc_token(token_len, token_start, line);
				if (!token_array[i])
					return (free_all(token_array, i));
				i++;
			}
		}
	}
	return (token_array[i] = NULL, token_array);
}

char	**ft_split_tokens(const char *line)
{
	size_t	cnt;
	char	**tkn_array;

	if (!line)
		return (NULL);
	cnt = ft_count_tokens_revised(line);
	tkn_array = malloc(sizeof(char *) * (cnt + 1));
	if (!tkn_array)
		return (NULL);
	if (!filltkn_array_revised(tkn_array, line))
		return (NULL);
	return (tkn_array);
}
