/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:26 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/15 16:34:11 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static int	extract_key_value_from_arg(char *arg, char **key_ptr,
										char **value_ptr, int *append_mode_ptr)
{
	char	*equals_ptr;
	size_t	key_len;

	equals_ptr = ft_strchr(arg, '=');
	if (equals_ptr != NULL)
	{
		key_len = equals_ptr - arg;
		if (key_len > 0 && arg[key_len - 1] == '+')
		{
			*append_mode_ptr = TRUE;
			*key_ptr = ft_substr(arg, 0, key_len - 1);
		}
		else
			*key_ptr = ft_substr(arg, 0, key_len);
		*value_ptr = ft_strdup(equals_ptr + 1);
		if (!*key_ptr || !*value_ptr)
			return (perror("minishell: export: malloc error"), -1);
	}
	else
	{
		*key_ptr = ft_strdup(arg);
		if (!*key_ptr)
			return (perror("minishell: export: malloc error"), -1);
	}
	return (EXIT_SUCCESS);
}

static	void	ft_prt_err(char *name)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

static int	validate_and_set_or_append_env(t_export_op *op_data)
{
	char	*curr_value;
	char	*f_vl_setenv;

	if (!ft_is_valid_identifier(op_data->key))
		return (ft_prt_err(op_data->original_arg_str), EXIT_FAILURE);
	f_vl_setenv = NULL;
	if (op_data->is_append_mode && op_data->value_to_process)
	{
		curr_value = ft_list_getenv(op_data->shell_dt->env_list, op_data->key);
		if (curr_value)
			f_vl_setenv = ft_strjoin(curr_value, op_data->value_to_process);
		else
			f_vl_setenv = ft_strdup(op_data->value_to_process);
		if (!f_vl_setenv)
			return (perror("mini: malloc err"), EXIT_FAILURE);
	}
	else if (op_data->value_to_process)
	{
		f_vl_setenv = ft_strdup(op_data->value_to_process);
		if (!f_vl_setenv) // Check strdup result
			return (perror("mini: malloc Err"), EXIT_FAILURE);
	}
	if (ft_list_setenv(&op_data->shell_dt->env_list, op_data->key, f_vl_setenv))
		return (perror("mini: setenv Err"), free(f_vl_setenv), EXIT_FAILURE);
	return (free(f_vl_setenv), EXIT_SUCCESS);
}

static int	process_export_arg(char *arg_str, t_data *data)
{
	t_export_op	op_data;
	int			result;

	op_data.key = NULL;
	op_data.value_to_process = NULL;
	op_data.original_arg_str = arg_str; // Store original for error messages
	op_data.shell_dt = data;// Pass data pointer
	op_data.is_append_mode = FALSE;
	result = extract_key_value_from_arg(arg_str, &op_data.key,
			&op_data.value_to_process, &op_data.is_append_mode);
	if (result == -1) // Malloc error during key/value extraction
	{
		free(op_data.key); // free(NULL) is safe
		free(op_data.value_to_process);
		return (EXIT_FAILURE);
	}
	result = validate_and_set_or_append_env(&op_data); // Pass struct pointer
	// Free the key and value strings extracted by extract_key_value_from_arg
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
