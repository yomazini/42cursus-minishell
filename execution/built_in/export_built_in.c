/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_built_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:26 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/14 22:43:04 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

// static int	validate_and_set_env(char *key,
// 	char *value, char *arg, t_data *data)
// {
// 	if (!ft_is_valid_identifier(key))
// 	{
// 		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
// 		ft_putstr_fd(arg, STDERR_FILENO);
// 		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
// 		return (EXIT_FAILURE);
// 	}
// 	if (ft_list_setenv(&data->env_list, key, value) == -1)
// 	{
// 		perror("minishell: export: setenv error");
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }

// static int	process_export_pair(char *arg, t_data *data)
// {
// 	char	*equals_ptr;
// 	char	*key;
// 	char	*value;
// 	int		result;

// 	equals_ptr = ft_strchr(arg, '=');
// 	key = NULL;
// 	value = NULL;
// 	result = EXIT_SUCCESS;
// 	if (equals_ptr != NULL)
// 	{
// 		key = ft_substr(arg, 0, equals_ptr - arg);
// 		value = ft_strdup(equals_ptr + 1);
// 		if (!key || !value)
// 			return (free(key), free(value), EXIT_FAILURE);
// 	}
// 	else
// 	{
// 		key = ft_strdup(arg);
// 		if (!key)
// 			return (perror("minishell: export: malloc error"), EXIT_FAILURE);
// 		value = NULL;
// 	}
// 	result = validate_and_set_env(key, value, arg, data);
// 	return (free(key), free(value), result);
// }

// int	ft_export(t_cmd *cmd, t_data *data)
// {
// 	int		i;
// 	int		return_status;

// 	return_status = EXIT_SUCCESS;
// 	if (cmd->argv[1] == NULL)
// 	{
// 		print_export_env(data->env_list);
// 		data->last_exit_status = EXIT_SUCCESS;
// 		return (EXIT_SUCCESS);
// 	}
// 	i = 1;
// 	while (cmd->argv[i] != NULL)
// 	{
// 		if (process_export_pair(cmd->argv[i], data) == EXIT_FAILURE)
// 			return_status = EXIT_FAILURE;
// 		i++;
// 	}
// 	data->last_exit_status = return_status;
// 	return (return_status);
// }



static int	validate_and_set_or_append_env(char *key, char *value_to_set_or_append,
										char *original_arg, t_data *data, int append_mode)
{
	char	*current_value;
	char	*new_value_for_setenv;

	if (!ft_is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(original_arg, STDERR_FILENO); // Print original arg for error
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}

	new_value_for_setenv = NULL;
	if (append_mode)
	{
		current_value = ft_list_getenv(data->env_list, key); // Get current value
		if (current_value) // If var exists and has a value, append
		{
			new_value_for_setenv = ft_strjoin(current_value, value_to_set_or_append);
		}
		else // Var doesn't exist or has no value, treat as normal set
		{
			new_value_for_setenv = ft_strdup(value_to_set_or_append);
		}
		if (!new_value_for_setenv)
		{
			perror("minishell: export (append): malloc error");
			return (EXIT_FAILURE);
		}
	}
	else // Not append mode (normal set or set without value)
	{
		if (value_to_set_or_append) // If a value is provided for set
			new_value_for_setenv = ft_strdup(value_to_set_or_append);
		// If value_to_set_or_append is NULL (e.g. export VAR), new_value_for_setenv remains NULL
	}

	// Call ft_list_setenv with the (potentially new or appended) value
	if (ft_list_setenv(&data->env_list, key, new_value_for_setenv) == -1)
	{
		perror("minishell: export: setenv error");
		free(new_value_for_setenv); // Free if allocated
		return (EXIT_FAILURE);
	}

	free(new_value_for_setenv); // Free the strduped/strjoined string for setenv
	return (EXIT_SUCCESS);
}

// Modified process_export_pair to detect and handle "+="
static int	process_export_pair(char *arg, t_data *data)
{
	char	*equals_ptr;
	char	*key;
	char	*value_to_assign;
	int		result;
	int		append_mode;
	size_t	key_len;

	equals_ptr = ft_strchr(arg, '=');
	key = NULL;
	value_to_assign = NULL;
	result = EXIT_SUCCESS;
	append_mode = FALSE;

	if (equals_ptr != NULL) // Argument contains "="
	{
		key_len = equals_ptr - arg;
		// Check if the character before '=' is '+'
		if (key_len > 0 && arg[key_len - 1] == '+')
		{
			append_mode = TRUE;
			key = ft_substr(arg, 0, key_len - 1); // Key is before "+="
		}
		else // Normal assignment "KEY=VALUE"
		{
			key = ft_substr(arg, 0, key_len);
		}
		value_to_assign = ft_strdup(equals_ptr + 1); // Value is after "="
		if (!key || !value_to_assign) // Check malloc results
		{
			perror("minishell: export: malloc error");
			result = EXIT_FAILURE;
		}
	}
	else // No '=' in argument, e.g., "export VAR"
	{
		key = ft_strdup(arg);
		if (!key)
		{
			perror("minishell: export: malloc error");
			result = EXIT_FAILURE;
		}
		// value_to_assign remains NULL
	}

	if (result == EXIT_SUCCESS) // If key/value extraction was okay
	{
		result = validate_and_set_or_append_env(key, value_to_assign, arg, data, append_mode);
	}

	free(key);
	free(value_to_assign); // free(NULL) is safe
	return (result);
}

// Main export function (No changes needed here, logic moved to helpers)
int	ft_export(t_cmd *cmd, t_data *data)
{
	int		i;
	int		return_status;

	return_status = EXIT_SUCCESS;
	if (cmd->argv[1] == NULL) // No arguments, print exported vars
	{
		// Assuming print_export_env is defined elsewhere and handles status
		print_export_env(data->env_list);
		data->last_exit_status = EXIT_SUCCESS;
		return (EXIT_SUCCESS);
	}

	// Process arguments
	i = 1;
	while (cmd->argv[i] != NULL)
	{
		if (process_export_pair(cmd->argv[i], data) == EXIT_FAILURE)
			return_status = EXIT_FAILURE; // If any arg fails, overall status is failure
		i++;
	}
	data->last_exit_status = return_status;
	return (return_status);
}

    
