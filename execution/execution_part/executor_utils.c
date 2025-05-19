/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:28:42 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/19 15:27:28 by ymazini          ###   ########.fr       */
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
	int		original_errno;

	set_signal_handlers_default();
	if (apply_redirections(cmd) != 0)
		exit(EXIT_FAILURE);
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (!cmd->argv || !cmd->argv[0])
			exit(EXIT_SUCCESS);
	}
	builtin_status = execute_built_ins(cmd, data);
	if (builtin_status != -1)
		exit(builtin_status);
	else
	{
		envp_array = convert_envlist_to_array(data->env_list);
		if (!envp_array)
			exit(EXIT_FAILURE);
		path = find_command_path(cmd->argv[0], data->env_list);
		if (!path)
		{
			original_errno = errno; // Capture errno from find_command_path
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
			if (original_errno == EISDIR)
				ft_putstr_fd(": is a directory\n", STDERR_FILENO);
			else if (original_errno == EACCES)
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			else
			{
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				(free_arr(envp_array), exit(127));
			}
			(free_arr(envp_array), exit(126));
		}
		execve(path, cmd->argv, envp_array);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		(perror(cmd->argv[0]), free(path), free_arr(envp_array));
		exit(126);
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
