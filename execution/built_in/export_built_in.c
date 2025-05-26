/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:26 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/26 18:41:11 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static int	set_or_append_value(t_export_op *op)
{
	char	*current_value_in_env;
	char	*newval;

	if (op->is_append_mode)
	{
		current_value_in_env = ft_list_getenv(op->shell_dt->env_list, op->key);
		if (current_value_in_env)
			newval = ft_strjoin(current_value_in_env, op->value_to_process);
		else
			newval = ft_strdup(op->value_to_process);
	}
	else
		newval = ft_strdup(op->value_to_process);
	if (!newval)
		return (perror("minishell: export: malloc"), EXIT_FAILURE);
	if (ft_list_setenv(&op->shell_dt->env_list, op->key, newval) == -1)
	{
		free(newval);
		return (perror("minishell: export pbl"), EXIT_FAILURE);
	}
	free(newval);
	return (EXIT_SUCCESS);
}

static int	set_no_value(t_export_op *op)
{
	char	*cur;

	cur = ft_list_getenv(op->shell_dt->env_list, op->key);
	if (cur)
		return (EXIT_SUCCESS);
	if (ft_list_setenv(&op->shell_dt->env_list, op->key, NULL) == -1)
		return (perror("minishell: export:"), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static	int	validate_and_set_or_append_env(t_export_op *op_data)
{
	if (!ft_is_valid_identifier(op_data->key))
	{
		ft_prt_err(op_data->original_arg_str);
		return (EXIT_FAILURE);
	}
	if (op_data->value_to_process)
		return (set_or_append_value(op_data));
	return (set_no_value(op_data));
}

static int	process_export_arg(char *arg_str, t_data *data)
{
	t_export_op	op_data;
	int			result;

	op_data.key = NULL;
	op_data.value_to_process = NULL;
	op_data.original_arg_str = arg_str;
	op_data.shell_dt = data;
	op_data.is_append_mode = FALSE;
	result = extract_key_value_from_arg(arg_str, &op_data.key,
			&op_data.value_to_process, &op_data.is_append_mode);
	if (result == -1)
	{
		free(op_data.key);
		free(op_data.value_to_process);
		return (EXIT_FAILURE);
	}
	result = validate_and_set_or_append_env(&op_data);
	free(op_data.key);
	free(op_data.value_to_process);
	return (result);
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
		if (process_export_arg(cmd->argv[i], data) == EXIT_FAILURE)
			return_status = EXIT_FAILURE;
		i++;
	}
	data->last_exit_status = return_status;
	return (return_status);
}
