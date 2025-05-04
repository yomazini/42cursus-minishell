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

static int	count_valid_env_entries(t_env *env_list)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = env_list;
	while (curr)
	{
		if (curr->name && curr->value)
			count++;
		curr = curr->next;
	}
	return (count);
}

static char	*create_env_string(t_env *node)
{
	char	*temp_join;
	char	*full_entry;

	temp_join = ft_strjoin(node->name, "=");
	if (!temp_join)
		return (NULL);
	full_entry = ft_strjoin(temp_join, node->value);
	free(temp_join);
	return (full_entry);
}

static int	fill_envp_array(char **envp_array, t_env *env_list, int count)
{
	t_env	*curr;
	int		i;

	curr = env_list;
	i = 0;
	while (curr && i < count)
	{
		if (curr->name && curr->value)
		{
			envp_array[i] = create_env_string(curr);
			if (!envp_array[i])
			{
				perror("minishell: malloc failed");
				free_arr(envp_array);
				return (-1);
			}
			i++;
		}
		curr = curr->next;
	}
	envp_array[i] = NULL;
	return (0);
}

char	**convert_envlist_to_array(t_env *env_list)
{
	int		count;
	char	**envp_array;

	count = count_valid_env_entries(env_list);
	envp_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp_array)
	{
		perror("minishell: malloc failed");
		return (NULL);
	}
	if (fill_envp_array(envp_array, env_list, count) == -1)
		return (NULL);
	return (envp_array);
}
