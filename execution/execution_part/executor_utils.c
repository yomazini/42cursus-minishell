/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:28:42 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/20 17:17:14 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

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

static int	try_builtins(t_cmd *cmd, t_data *data)
{
	int	status;

	status = execute_built_ins(cmd, data);
	if (status != -1)
		exit(status);
	return (-1);
}

static void	run_external(t_cmd *cmd, t_data *data)
{
	char	*path;
	char	**envp;
	int		errno_saved;

	envp = convert_envlist_to_array(data->env_list);
	if (!envp)
		exit(EXIT_FAILURE);
	errno = 0;
	path = find_command_path(cmd->argv[0], data->env_list);
	errno_saved = errno;
	if (!path)
		handle_path_error_and_exit(cmd->argv[0], envp, errno_saved);
	execve(path, cmd->argv, envp);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(cmd->argv[0]);
	free(path);
	free_arr(envp);
	exit(126);
}

void	execute_command_node(t_cmd *cmd, t_data *data)
{
	set_signal_handlers_default();
	if (apply_redirections(cmd) != 0)
		exit(EXIT_FAILURE);
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (!cmd->argv || !cmd->argv[0])
			exit(EXIT_SUCCESS);
	}
	if (cmd->argv[0])
	{
		if (ft_strncmp(cmd->argv[0], ".", 2) == 0)
		{
			ft_putstr_fd("minishell: .: filename argument required\n", 2);
			exit(2);
		}
		if (ft_strncmp(cmd->argv[0], "..", 3) == 0)
		{
			ft_putstr_fd("minishell: ..: command not found\n", STDERR_FILENO);
			exit(127);
		}
	}
	if (try_builtins(cmd, data) >= 0)
		return ;
	run_external(cmd, data);
}
