/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazinistudent.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:09:01 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/22 15:59:47 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

char	**convert_envlist_to_array(t_env *env_list)
{
	int		count;
	t_env	*curr;
	char	**envp_array;
	char	*temp_join;
	int		i;

	count = 0;
	curr = env_list;
	while (curr)
	{
		if (curr->name && curr->value)
			count++;
		curr = curr->next;
	}
	envp_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp_array)
	{
		perror("minishell: malloc failed in convert_envlist_to_array");
		return (NULL);
	}
	curr = env_list;
	i = 0;
	while (curr && i < count)
	{
		if (curr->name && curr->value)
		{
			temp_join = ft_strjoin(curr->name, "=");
			if (!temp_join)
			{
				free_arr(envp_array);
				perror("minishell: malloc failed in convert_envlist_to_array");
				return (NULL);
			}
			envp_array[i] = ft_strjoin(temp_join, curr->value);
			free(temp_join);
			if (!envp_array[i])
			{
				free_arr(envp_array);
				perror("minishell: malloc failed in convert_envlist_to_array");
				return (NULL);
			}
			i++;
		}
		curr = curr->next;
	}
	envp_array[i] = NULL;
	return (envp_array);
}
