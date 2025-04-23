/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:43:13 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/23 12:58:47 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	ft_isoperater(int c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	ft_isdouble_op(const char *line, int k)
{
	if ((line[k] == '>' && line[k + 1] == '>')
		|| (line[k] == '<' && line[k + 1] == '<'))
	{
		return (1);
	}
	return (0);
}

char	**free_all(char **tkn_array, int i)
{
	while (i >= 0)
	{
		free(tkn_array[i]);
		i--;
	}
	free(tkn_array);
	return (NULL);
}

char	*ft_alloc_token(int token_len, int j, const char *line)
{
	char	*token_str;

	token_str = malloc(sizeof(char) *(token_len + 1));
	if (!token_str)
		return (NULL);
	ft_strncpy(token_str, &line[j], token_len);
	token_str[token_len] = '\0';
	return (token_str);
}

void ft_advance_word_token(const char *line, int *i)
{
	char	quote;
	
	while (line[*i])
	{
		if (line[*i] == '$' && line[*i + 1] && ft_isquot(line[*i + 1]))
		{
			quote = line[*i + 1];
			*i += 2;
			while (line[*i] && line[*i] != quote)
				(*i)++;
			if (line[*i] == quote)
				(*i)++;
			continue ;
		}
		if (ft_isspace(line[*i]) || ft_isoperater(line[*i]) || ft_isquot(line[*i]))
			break ;
		(*i)++;
	}
}
