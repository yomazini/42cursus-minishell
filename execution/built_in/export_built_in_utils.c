/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:21:36 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/04 15:08:41 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static void	ft_assign_var(int *count, int *i)
{
	*count = 0;
	*i = -1;
}

void	ft_sort_array(char **array)
{
	int		i;
	int		j;
	int		count;
	char	*temp;

	ft_assign_var(&count, &i);
	if (!array)
		return ;
	while (array[count])
		count++;
	if (count < 2)
		return ;
	while (++i < count - 1)
	{
		j = -1;
		while (++j < count - i - 1)
		{
			if (ft_strncmp(array[j], array[j + 1], -1) > 0)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
		}
	}
}

static void	print_single_export_entry(char *env_entry)
{
	char	*equals_ptr;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	equals_ptr = ft_strchr(env_entry, '=');
	if (equals_ptr)
	{
		write(STDOUT_FILENO, env_entry, equals_ptr - env_entry + 1);
		ft_putchar_fd('"', STDOUT_FILENO);
		ft_putstr_fd(equals_ptr + 1, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd(env_entry, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
}

void	print_export_env(t_env *env_list)
{
	char	**env_array;
	int		i;
	int		status;

	status = EXIT_SUCCESS;
	env_array = ft_list_to_array(env_list);
	if (!env_array)
	{
		perror("minishell: export: malloc error");
		return ;
	}
	ft_sort_array(env_array);
	i = 0;
	while (env_array[i] != NULL)
	{
		print_single_export_entry(env_array[i]);
		i++;
	}
	free_arr(env_array);
}
