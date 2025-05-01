/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:26 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/01 14:53:40 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	ft_is_valid_identifier(const char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char **ft_list_to_array(t_env *env_list)
{
	int		count;
	t_env	*curr;
	char	**arr;
	char	*tmp;
	int		i;

	// ft_putstr_fd("DEBUG: Entering ft_list_to_array\n", 2); 
	count = 0;
	curr = env_list;
	while (curr)
	{
		if (curr->name && curr->value)
			count++;
		curr = curr->next;
	}
	// ft_putstr_fd("DEBUG: Counted variables with values: ", 2);
	// fprintf(stderr, "%d\n", count);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr) {
        // perror("DEBUG: Malloc failed for array");
        return (NULL);
    }
	// ft_putstr_fd("DEBUG: Allocated array\n", 2);

	// --- Second Pass: Construct strings ---
	curr = env_list;
	i = 0;
	while (curr && i < count)
	{
		if (curr->name && curr->value)
		{
			tmp = ft_strjoin(curr->name, "=");
			if (!tmp) { free_arr(arr); return (NULL); }
			arr[i] = ft_strjoin(tmp, curr->value);
			free(tmp);
			if (!arr[i]) { free_arr(arr); return (NULL); }
			i++;
		}
		curr = curr->next;
	}
	arr[i] = NULL;

	// ft_putstr_fd("DEBUG: Exiting ft_list_to_array successfully\n", 2);
	return (arr);
}

void ft_sort_array(char **array)
{
	int i, j, count = 0;
	char *temp;

	if (!array) return;
	while (array[count]) count++;
	if (count < 2) return;

	// ft_putstr_fd("DEBUG: Entering ft_sort_array\n", 2);

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(array[j], array[j + 1], -1) > 0)
			{
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	// ft_putstr_fd("DEBUG: Exiting ft_sort_array\n", 2);
}

static void	print_export_env(t_env *env_list)
{
	char	**env_array;
	int		i;
	char	*equals_ptr;

	env_array = ft_list_to_array(env_list);
	if (!env_array)
	{
		perror("minishell: export: malloc error");
		return;
	}
	ft_sort_array(env_array);
	i = 0;
	while (env_array[i])
	{
		equals_ptr = ft_strchr(env_array[i], '=');
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		if (equals_ptr)
		{
			write(STDOUT_FILENO, env_array[i], equals_ptr - env_array[i] + 1);
			ft_putchar_fd('"', STDOUT_FILENO);
			ft_putstr_fd(equals_ptr + 1, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd(env_array[i], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		i++;
	}
	free_arr(env_array);
}

int	ft_export(t_cmd *cmd, t_data *data)
{
	int		i;
	int		return_status;
	char	*arg;
	char	*key;
	char	*value;
	char	*equals_ptr;
	return_status = EXIT_SUCCESS;

	if (cmd->argv[1] == NULL)
	{
		print_export_env(data->env_list);
		data->last_exit_status = EXIT_SUCCESS;
		return (EXIT_SUCCESS);
	}
	i = 1;
	while (cmd->argv[i] != NULL)
	{
		arg = cmd->argv[i];
		equals_ptr = ft_strchr(arg, '=');
		key = NULL;
		value = NULL;
		if (equals_ptr != NULL)
		{
			key = ft_substr(arg, 0, equals_ptr - arg);
			value = ft_strdup(equals_ptr + 1);
			if (!key || !value)
			{
				perror("minishell: export: malloc error");
				free(key); free(value);
				return_status = EXIT_FAILURE;
				i++;
				continue;
			}
		}
		else
		{
			key = ft_strdup(arg);
			if (!key) 
			{
				 perror("minishell: export: malloc error");
				 return_status = EXIT_FAILURE;
					i++; 
				   continue;
			}
			value = NULL;
		}
		if (!ft_is_valid_identifier(key))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return_status = EXIT_FAILURE;
		}
		else
		{
			if (ft_list_setenv(&data->env_list, key, value) == -1)
			{
				perror("minishell: export: setenv error");
				return_status = EXIT_FAILURE;
			}
		}
		free(key);
		free(value);
		i++;
	}
	data->last_exit_status = return_status;
	return (return_status);
}
