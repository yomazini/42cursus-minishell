/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:32 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/16 18:16:15 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

static void	skip_token(const char *line, int *i)
{
	char	quote;

	if (ft_isquot(line[*i]))
	{
		quote = line[*i];
		(*i)++;
		while (line[*i] && line[*i] != quote)
			(*i)++;
		if (line[*i] == quote)
			(*i)++;
	}
	else if (ft_isoperater(line[*i]))
	{
		if (ft_isdouble_op(line, *i))
			*i += 2;
		else
			(*i)++;
	}
	else
		while (line[*i] && !ft_isspace(line[*i])
			&& !ft_isquot(line[*i]) && !ft_isoperater(line[*i]))
			(*i)++;
}

static size_t	ft_cnt_tokens(const char *line)
{
	int		index;
	size_t	cnt_tokens;

	index = 0;
	cnt_tokens = 0;
	while (line[index])
	{
		while (line[index] && ft_isspace(line[index]))
			index++;
		if (line[index])
		{
			cnt_tokens++;
			skip_token(line, &index);
		}
	}
	return (cnt_tokens);
}

static char	*extract_token(const char *line, int *k)
{
	int		j;
	int		token_len;
	char	quote_char;

	j = *k;
	if (ft_isquot(line[j]))
	{
		quote_char = line[(*k)++];
		while (line[*k] && line[*k] != quote_char)
			(*k)++;
		if (line[*k] == quote_char)
			(*k)++;
	}
	else if (ft_isoperater(line[j]))
		*k += 1 + ft_isdouble_op(line, j);
	else
		while (line[*k] && !ft_isspace(line[*k])
			&& !ft_isquot(line[*k]) && !ft_isoperater(line[*k]))
			(*k)++;
	token_len = *k - j;
	if (token_len <= 0)
		token_len = 0;
	return (ft_alloc_token(token_len, j, line));
}

static char	**filltkn_array(char **token_array, const char *line)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (line[k])
	{
		while (line[k] && ft_isspace(line[k]))
			k++;
		if (line[k])
		{
			token_array[i] = extract_token(line, &k);
			if (!token_array[i])
				free_all(token_array, i - 1);
			i++;
		}
	}
	token_array[i] = NULL;
	return (token_array);
}

char	**ft_split_tokens(const char *line)
{
	size_t	cnt;
	char	**tkn_array;

	cnt = ft_cnt_tokens(line);
	tkn_array = malloc(sizeof(char *) * (cnt + 1));
	if (!tkn_array)
		return (NULL);
	if (!filltkn_array(tkn_array, line))
		return (NULL);
	return (tkn_array);
}
