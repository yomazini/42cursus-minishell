/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:28:42 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/18 11:39:28 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	is_parent_builtin(t_cmd *cmd)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (FALSE);
	name = cmd->argv[0];
	if ((ft_strncmp(name, "pwd", 4) == 0)
		|| (ft_strncmp(name, "echo", 5) == 0)
		|| (ft_strncmp(name, "env", 4) == 0)
		|| (ft_strncmp(name, "exit", 5) == 0)
		|| (ft_strncmp(name, "cd", 3) == 0)
		|| (ft_strncmp(name, "unset", 6) == 0)
		|| (ft_strncmp(name, "export", 7) == 0))
		return (TRUE);
	return (FALSE);
}

// static void	ft_check_command_existance(char *path,
// 			char *cmd_name, char **envp_array)
// {
// 	if (!path)
// 	{
// 		ft_putstr_fd("minishell3: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd_name, STDERR_FILENO);
// 		ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 		free_arr(envp_array);
// 		exit(127);
// 	}
// }

// Updated helper for path checking errors
// This function will now *exit* the child if the path is invalid.
void	handle_path_error_and_exit(char *cmd_name, char **envp_array,
									int original_errno)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	if (original_errno == EISDIR) // Set by is_executable if it was a directory
	{
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		free_arr(envp_array); // Free envp_array if it was allocated
		exit(126);
	}
	else if (original_errno == EACCES) // Set by is_executable for file permission
	{
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		free_arr(envp_array);
		exit(126);
	}
	else // Default "command not found" (ENOENT or other errors from find_path)
	{
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_arr(envp_array);
		exit(127);
	}
}


// void	execute_command_node(t_cmd *cmd, t_data *data)
// {
// 	int		builtin_status;
// 	char	*path;
// 	char	**envp_array;
// 	set_signal_handlers_default(); // Resets SIGINT and SIGQUIT to SIG_DFL
// 	if (apply_redirections(cmd) != 0)
// 		exit(EXIT_FAILURE);
		

// 	if (!cmd->argv || !cmd->argv[0])
// 	{
// 		ft_putstr_fd("minishell: Invalid command node in pipeline\n", 2);
// 		exit(EXIT_FAILURE);
// 	}
	
	
// 	builtin_status = execute_built_ins(cmd, data);
// 	if (builtin_status != -1)
// 		exit(builtin_status);
// 	else
// 	{
// 		envp_array = convert_envlist_to_array(data->env_list);
// 		if (!envp_array)
// 			exit(EXIT_FAILURE);
// 			// --- Handle "." and ".." specially as per Bash for direct execution ---
// 		if (cmd->argv[0] && ft_strncmp(cmd->argv[0], ".", 2) == 0)
// 		{
// 			ft_putstr_fd("minishell: .: filename argument required\n", STDERR_FILENO);
// 			ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
// 			data->last_exit_status = 2;
// 			(free_arr(envp_array), exit(2));
// 		}
// 		if (cmd->argv[0] && ft_strncmp(cmd->argv[0], "..", 3) == 0)
// 		{
// 			ft_putstr_fd("minishell: ..: command not found\n", STDERR_FILENO);
// 			data->last_exit_status = 127;
// 			(free_arr(envp_array), exit(127));
// 		}
// 		// --- End special handling for . and .. ---

// 		errno = 0; // Clear errno before calling find_command_path
// 		path = find_command_path(cmd->argv[0], data->env_list);
// 		int find_path_errno = errno; // Capture errno immediately after find_command_path		ft_check_command_existance(path, cmd->argv[0], envp_array);
// 			if (!path) // Path not found or not executable or is a directory
// 	{
// 		// handle_path_error_and_exit will print message and exit child
// 		handle_path_error_and_exit(cmd->argv[0], envp_array, find_path_errno);
// 		// The line below is never reached as handle_path_error_and_exit exits
// 	}

// 		execve(path, cmd->argv, envp_array);
// 		ft_putstr_fd("minishell1: ", STDERR_FILENO);
// 		(perror(cmd->argv[0]), free(path), free_arr(envp_array), exit(126));
// 	}
// }


void	execute_command_node(t_cmd *cmd, t_data *data)
{
	int		builtin_status;
	char	*path;
	char	**envp_array;

	set_signal_handlers_default(); // Reset child signals

	if (apply_redirections(cmd) != 0) // Apply redirections first
		exit(EXIT_FAILURE);          // Exit if redirection fails

	// --- FIX: Handle Empty/Invalid argv[0] for this node ---
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		// If argv[0] is NULL or an empty string "", it's an empty command segment.
		// Bash typically exits 0 for this in a pipeline (it doesn't break the pipe).
		// If argv[0] was literally "" (from "$EMPTY_VAR_QUOTED"),
		// find_command_path will return NULL and trigger "command not found".
		// This check is for when no command word was formed at all.
		if (!cmd->argv || !cmd->argv[0]) // Truly no argv array or first element
		{
			exit(EXIT_SUCCESS); // Successfully did nothing, let pipe continue
		}
		// If argv[0] is "" (empty string), fall through to command execution attempt.
	}
	// --- END FIX ---

	// Try to execute as builtin
	builtin_status = execute_built_ins(cmd, data);
	if (builtin_status != -1) // It was a builtin
		exit(builtin_status);   // Child exits with builtin's status
	else // Not a builtin, proceed with external execution
	{
		envp_array = convert_envlist_to_array(data->env_list);
		if (!envp_array)
			exit(EXIT_FAILURE); // Malloc error in child

		path = find_command_path(cmd->argv[0], data->env_list);
		// ft_check_command_existance will exit if path is NULL
		// No need to duplicate its logic here, just call it.
		// Ensure ft_check_command_existance handles envp_array freeing correctly.

		// --- FIX: Let ft_check_command_existance handle cmd not found / is dir / permission ---
		// The original ft_check_command_existance needs to be in executor_utils.c
		// and take find_path_errno as an argument if we want to use that.
		// For now, find_command_path sets errno.
		if (!path) // path not found or not executable
		{
			int original_errno = errno; // Capture errno from find_command_path
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
			if (original_errno == EISDIR)
				ft_putstr_fd(": is a directory\n", STDERR_FILENO);
			else if (original_errno == EACCES)
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			else
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
			free_arr(envp_array);
			exit(original_errno == EACCES || original_errno == EISDIR ? 126 : 127);
		}
		// --- END FIX ---

		execve(path, cmd->argv, envp_array);

		// execve failed if we reach here
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd->argv[0]); // Will use errno set by execve
		free(path);
		free_arr(envp_array);
		exit(errno == EACCES ? 126 : EXIT_FAILURE); // Use actual errno
	}
}


void	execute_child_process(t_cmd *cmd, t_data *data, char *path)
{
	char	**envp_array;

	if (apply_redirections(cmd) != 0)
	{
		free(path);
		exit(EXIT_FAILURE);
	}
	envp_array = convert_envlist_to_array(data->env_list);
	if (!envp_array)
	{
		ft_putstr_fd("minishell: child: env setup failed\n", STDERR_FILENO);
		free(path);
		exit(EXIT_FAILURE);
	}
	execve(path, cmd->argv, envp_array);
	ft_putstr_fd("minishell2: ", STDERR_FILENO);
	perror(cmd->argv[0]);
	free(path);
	free_arr(envp_array);
	exit(126);
}
