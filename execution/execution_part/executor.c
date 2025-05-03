/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:30:20 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/02 23:24:04 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

      
int	execute_commands(t_cmd *cmd_list, t_data *data)
{
	int		command_count;
	t_cmd	*counter;
	// int		status; // Variable to store return status

	if (!cmd_list) 
		return (data->last_exit_status = 0, 0);

	// Count commands
	command_count = 0;
	counter = cmd_list;
	while (counter != NULL) { command_count++; counter = counter->next; }

	// --- Dispatch ---
	if (command_count == 1)
	{
		// --- FIX: Check for valid argv before proceeding ---
		if (!cmd_list->argv || !cmd_list->argv[0])
		{
            // Command exists but has no command word (e.g., just redirections)
            // Apply redirections only? Bash usually errors here.`
            // Let's apply redirections and return error status (1)
            // Or just return an error directly? Let's apply for now.
            ft_putstr_fd("minishell: syntax error near unexpected token `newline' (or invalid command)\n", 2); // Or specific error
            // status = apply_redirections(cmd_list); // Apply redirections?
			// data->last_exit_status = (status == 0) ? 0 : 1; // Status 0 if redir ok? Bash errors.
			data->last_exit_status = 2; // Syntax error is safer
			return (data->last_exit_status);
		}
		// --- End FIX ---

		// Now we know argv[0] exists
		if (is_parent_builtin(cmd_list)) // Check if it's cd, export <args>, unset, exit
		{
			execute_built_ins(cmd_list, data);
		}
		else // External or child-safe builtin
		{
			execute_external_command(cmd_list, data);
		}
	}
	else // command_count > 1 -> Pipeline
	{
		// --- FIX: Check first command in pipeline ---
		if (!cmd_list->argv || !cmd_list->argv[0]) {
            ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
            data->last_exit_status = 2; // Syntax error
            return(data->last_exit_status);
        }
        // --- End FIX ---
		execute_pipeline(cmd_list, data);
	}

	return (data->last_exit_status);
}

 void	execute_command_node(t_cmd *cmd, t_data *data)
{
	int		builtin_status;
	char	*path;
	char	**envp_array;

	// Apply redirections first
	if (apply_redirections(cmd) != 0)
		exit(EXIT_FAILURE);

	// --- FIX: Check if command exists before trying builtins/externals ---
	if (!cmd->argv || !cmd->argv[0])
	{
		// This case implies an error like `cmd1 | > file | cmd3` where the middle
		// command node has no actual command. Parser should ideally prevent this.
		// If it happens, exit child with error.
		ft_putstr_fd("minishell: Invalid command node in pipeline\n", 2);
		exit(EXIT_FAILURE); // Or syntax error code?
	}
	// --- End FIX ---

	// Now safe to check cmd->argv[0]
	builtin_status = execute_built_ins(cmd, data);

	if (builtin_status != -1) // It was a builtin
		exit(builtin_status);
	else // External command
	{
		envp_array = convert_envlist_to_array(data->env_list);
		if (!envp_array) exit(EXIT_FAILURE);
		path = find_command_path(cmd->argv[0], data->env_list);
		if (!path)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			free_arr(envp_array);
			exit(127);
		}
		execve(path, cmd->argv, envp_array);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd->argv[0]);
		free(path);
		free_arr(envp_array);
		exit(errno == EACCES ? 126 : EXIT_FAILURE);

	}
}


int	execute_external_command(t_cmd *cmd, t_data *data)
{
	char	*executable_path;
	char	**envp_array;
	pid_t	child_pid;
	int		wait_status;
	// int		exit_code;

	executable_path = NULL;
	envp_array = NULL;
	executable_path = find_command_path(cmd->argv[0], data->env_list);
	if (!executable_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
        ft_putstr_fd(" : command not found\n", STDERR_FILENO);
		data->last_exit_status = 127;
		return (127);
	}
	child_pid = fork();
	if (child_pid < 0)
	{
		perror("minishell: fork");
		free(executable_path);
		data->last_exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	else if (child_pid == 0)
	{
		if (apply_redirections(cmd) != 0)
		{
			free(executable_path);
			executable_path = NULL;
			exit(EXIT_FAILURE);
		}
		envp_array = convert_envlist_to_array(data->env_list);
		if (!envp_array)
		{
			ft_putstr_fd("minishell: child: env setup failed\n", STDERR_FILENO);
			free(executable_path);
			exit(EXIT_FAILURE);
		}
		execve(executable_path, cmd->argv, envp_array);
		// execve failed if we reach here Below: 
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd->argv[0]);
		free(executable_path);
		free_arr(envp_array);
		exit(126);
	}
	else
	{
		free(executable_path);
		executable_path = NULL;
		wait_status = 0;
		waitpid(child_pid, &wait_status, 0);
		update_last_exit_status(data, wait_status);
		return (data->last_exit_status);
	}
}

int	is_parent_builtin(t_cmd *cmd)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (FALSE);
	name = cmd->argv[0];
		if ((ft_strncmp(name, "pwd", 4) == 0) ||
		(ft_strncmp(name, "echo", 5) == 0) ||
		(ft_strncmp(name, "env", 4) == 0) ||
		(ft_strncmp(name, "exit", 5) == 0) ||
        (ft_strncmp(name, "cd", 3) == 0) ||
        (ft_strncmp(name, "unset", 6) == 0) ||
        (ft_strncmp(name, "export", 7) == 0))
			return TRUE;
	return (FALSE);
}
