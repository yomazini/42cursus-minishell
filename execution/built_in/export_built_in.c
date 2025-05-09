/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:26 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/09 00:34:07 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static int	validate_and_set_env(char *key,
	char *value, char *arg, t_data *data)
{
	if (!ft_is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (ft_list_setenv(&data->env_list, key, value) == -1)
	{
		perror("minishell: export: setenv error");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	process_export_pair(char *arg, t_data *data)
{
	char	*equals_ptr;
	char	*key;
	char	*value;
	int		result;

	equals_ptr = ft_strchr(arg, '=');
	key = NULL;
	value = NULL;
	result = EXIT_SUCCESS;
	if (equals_ptr != NULL)
	{
		key = ft_substr(arg, 0, equals_ptr - arg);
		value = ft_strdup(equals_ptr + 1);
		if (!key || !value)
			return (free(key), free(value), EXIT_FAILURE);
	}
	else
	{
		key = ft_strdup(arg);
		if (!key)
			return (perror("minishell: export: malloc error"), EXIT_FAILURE);
		value = NULL;
	}
	result = validate_and_set_env(key, value, arg, data);
	return (free(key), free(value), result);
}

int	ft_export(t_cmd *cmd, t_data *data)
{
	int		i;
	int		return_status;

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
		if (process_export_pair(cmd->argv[i], data) == EXIT_FAILURE)
			return_status = EXIT_FAILURE;
		i++;
	}
	data->last_exit_status = return_status;
	return (return_status);
}
