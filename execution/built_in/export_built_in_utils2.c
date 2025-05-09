/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:26:53 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/09 16:47:53 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

// static int	count_env_nodes(t_env *env_list)
// {
// 	int		count;
// 	t_env	*curr;

// 	count = 0;
// 	curr = env_list;
// 	while (curr)
// 	{
// 		if (curr->name)
// 			count++;
// 		curr = curr->next;
// 	}
// 	return (count);
// }

// static char	*create_env_entry_str(t_env *node)
// {
// 	char	*entry;
// 	char	*tmp;

// 	if (node->value != NULL)
// 	{
// 		tmp = ft_strjoin(node->name, "=");
// 		if (!tmp)
// 			return (NULL);
// 		entry = ft_strjoin(tmp, node->value);
// 		free(tmp);
// 	}
// 	else
// 		entry = ft_strdup(node->name);
// 	return (entry);
// }

// static int	fill_env_array(char **arr, t_env *env_list, int count)
// {
// 	t_env	*curr;
// 	int		i;

// 	curr = env_list;
// 	i = 0;
// 	while (curr && i < count)
// 	{
// 		if (curr->name)
// 		{
// 			arr[i] = create_env_entry_str(curr);
// 			if (!arr[i])
// 			{
// 				free_arr(arr);
// 				return (0);
// 			}
// 			i++;
// 		}
// 		curr = curr->next;
// 	}
// 	arr[i] = NULL;
// 	return (1);
// }

// char	**ft_list_to_array(t_env *env_list)
// {
// 	int		count;
// 	char	**arr;

// 	count = count_env_nodes(env_list);
// 	arr = malloc(sizeof(char *) * (count + 1));
// 	if (!arr)
// 	{
// 		perror("minishell: export: malloc failed in list_to_array");
// 		return (NULL);
// 	}
// 	if (!fill_env_array(arr, env_list, count))
// 		return (NULL);
// 	return (arr);
// }
