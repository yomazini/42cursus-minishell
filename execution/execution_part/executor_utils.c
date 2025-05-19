/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:28:42 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/19 22:14:51 by ymazini          ###   ########.fr       */
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

void	handle_path_error_and_exit(char *cmd_name, char **envp_array,
									int original_errno)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	if (original_errno == EISDIR)
	{
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		free_arr(envp_array);
		exit(126);
	}
	else if (original_errno == EACCES)
	{
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		free_arr(envp_array);
		exit(126);
	}
	else
	{
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_arr(envp_array);
		exit(127);
	}
}

void	execute_command_node(t_cmd *cmd, t_data *data)
{
	int		builtin_status;
	char	*path;
	char	**envp_array;
	int		original_errno; // For find_command_path errors

	set_signal_handlers_default(); // Reset child signals
	if (apply_redirections(cmd) != 0) // Apply redirections first
		exit(EXIT_FAILURE);          // Exit if redirection fails

	// --- Handle Empty/Invalid argv[0] for this node in pipeline ---
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		// If argv[0] is NULL or "", this segment has no command to execute.
		// For example, `ls | "" | wc` or `ls | >out | wc`.
		// The redirections for this segment are done.
		// This child should exit successfully so the pipe doesn't break prematurely.
		// If it was `""`, find_command_path will correctly fail later.
		// If it was truly no command (argv[0] is NULL), exit 0.
		if (!cmd->argv || !cmd->argv[0])
			exit(EXIT_SUCCESS); // Successfully did nothing (only redirections)
		// If argv[0] is "", let it fall through to command execution attempt
	}

	// Special handling for "." and ".." as commands WITHIN a pipeline
	if (cmd->argv[0])
	{
		if (ft_strncmp(cmd->argv[0], ".", 2) == 0)
		{
			ft_putstr_fd("minishell: .: filename argument required\n", STDERR_FILENO);
			// data->last_exit_status = 2; // Not needed here, child exits
			exit(2);
		}
		if (ft_strncmp(cmd->argv[0], "..", 3) == 0)
		{
			ft_putstr_fd("minishell: ..: command not found\n", STDERR_FILENO);
			// data->last_exit_status = 127;
			exit(127);
		}
	}
	// --- End special handling ---
	// Try to execute as builtin (child-safe ones or if it's the only cmd)
	builtin_status = execute_built_ins(cmd, data);
	if (builtin_status != -1) // It was a builtin
		exit(builtin_status);   // Child exits with builtin's status
	else // Not a builtin, proceed with external execution
	{
		envp_array = convert_envlist_to_array(data->env_list);
		if (!envp_array)
			exit(EXIT_FAILURE); // Malloc error in child

		errno = 0; // Clear errno
		path = find_command_path(cmd->argv[0], data->env_list);
		original_errno = errno; // Capture errno from find_command_path

		if (!path) // path not found or not executable
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
			if (original_errno == EISDIR)
			{
				free_arr(envp_array);
				ft_putstr_fd(": is a directory\n", STDERR_FILENO);
				exit(126);
			}
			else if (original_errno == EACCES)
			{
				free_arr(envp_array);
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);		
				exit(126);
			}
			else
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
			free_arr(envp_array);
			exit(127);
		}
		execve(path, cmd->argv, envp_array);

		// execve failed if we reach here
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd->argv[0]); // Will use errno set by execve
		free(path);
		free_arr(envp_array);
		exit(126); // Use actual errno
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
