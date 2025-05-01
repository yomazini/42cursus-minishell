/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_converter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:02:10 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/01 15:18:16 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h" 

// static int	count_word_tokens_simple(t_token *token_list)
// {
// 	int		count = 0;
// 	t_token	*current = token_list;

// 	while (current)
// 	{
// 		if (current->type == TOKEN_WORD)
// 			count++;
// 		current = current->next;
// 	}
// 	return (count);
// }

// static char	**create_argv_from_simple_tokens(t_token *token_list, int word_count)
// {
// 	char	**argv_array;
// 	t_token	*current = token_list;
// 	int		i = 0;

// 	argv_array = (char **)malloc(sizeof(char *) * (word_count + 1));
// 	if (!argv_array)
// 		return (NULL);
// 	while (current)
// 	{
// 		if (current->type == TOKEN_WORD)
// 		{
// 			argv_array[i] = ft_strdup(current->value);
// 			if (!argv_array[i])
// 			{
// 				free_arr(argv_array);
// 				return (NULL);
// 			}
// 			i++;
// 		}
// 		current = current->next;
// 	}
// 	argv_array[i] = NULL;
// 	return (argv_array);
// }

void	free_cmd_struct(t_cmd *cmd)
{
	if (!cmd)
		return;
	free_arr(cmd->argv);
	free(cmd);
}
