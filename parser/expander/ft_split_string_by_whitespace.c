/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_string_by_whitespace.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:44:32 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/17 09:44:00 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	**ft_free(char **string, int size)
{
	while (size--)
		free(string[size]);
	free(string);
	return (NULL);
}
static int	count_worlds_by_whitespace(const char *s)
{
	int	count;
	int	i;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
		{
			if (in_word == 0)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		i++;
	}
	return (count);	
}

static char	*get_next_world(const char *s, size_t *start_index)
{
	char	*word;
	size_t	end_word;
	size_t	curr_pos;

	curr_pos = *start_index;
	while (s[curr_pos] && ft_isspace(s[curr_pos]))
		curr_pos++;
	if (s[curr_pos] == '\0')
		return (NULL);
	end_word = curr_pos;
	while (s[end_word] && !ft_isspace(s[end_word]))
		end_word++;
	word = malloc(sizeof(char) * (end_word - curr_pos + 1));
	if (!word)
		return (NULL);
	ft_strncpy(word, s + curr_pos, (end_word - curr_pos));
	word[end_word - curr_pos] = '\0';
	*start_index = end_word;
	return (word);
}
char	**ft_split_string_by_whitespace(char *string, size_t *size)
{
	char	**res_str;
	int		n_word;
	size_t	curr_index;
	int		i;

	if (!string)
		return (NULL);
	n_word = count_worlds_by_whitespace(string);
	if (n_word == 0)
		return (NULL);
	res_str = malloc(sizeof(char *) * (n_word + 1));
	if (!res_str)
		return (NULL);
	i = 0;
	curr_index = 0;
	while(i < n_word)
	{
		res_str[i] = get_next_world(string, &curr_index);
		if (!res_str[i])
			return (ft_free(res_str, i));
		i++;
	}
	res_str[i] = NULL;
	return (*size = n_word, res_str);
}
