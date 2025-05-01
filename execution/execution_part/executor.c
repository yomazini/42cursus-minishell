/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:30:20 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/01 15:18:42 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

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
	// if (ft_strncmp(name, "cd", 3) == 0) return (TRUE);
	// if (ft_strncmp(name, "export", 7) == 0 && cmd->argv[1] != NULL)
	// 	 return (TRUE);
	// if (ft_strncmp(name, "unset", 6) == 0) return (TRUE);
	// if (ft_strncmp(name, "exit", 5) == 0) return (TRUE);
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

int	is_known_builtin_name(char *name)
{
	if (!name) return (FALSE);
	if ((ft_strncmp(name, "pwd", 4) == 0) ||
		(ft_strncmp(name, "echo", 5) == 0) ||
		(ft_strncmp(name, "env", 4) == 0) ||
		(ft_strncmp(name, "exit", 5) == 0) ||
        (ft_strncmp(name, "cd", 3) == 0) ||
        (ft_strncmp(name, "unset", 6) == 0) ||
        (ft_strncmp(name, "export", 7) == 0))
		return (TRUE);
	return (FALSE);
}


int	execute_commands(t_cmd *cmd_list, t_data *data)
{
	int		command_count;
	t_cmd	*counter;

	if (!cmd_list) return (data->last_exit_status = 0, 0);
	command_count = 0;
	counter = cmd_list;
	while (counter != NULL) { command_count++; counter = counter->next; }

	if (command_count == 1)
	{
		if (is_parent_builtin(cmd_list) || 1)
			execute_built_ins(cmd_list, data);
		else // External command OR child-safe builtin (echo, pwd, env, export no-args)
		{
			// Let external executor handle fork/redir/exec/wait
			// It should also handle calling builtins within the child if needed
			execute_external_command(cmd_list, data);
		}
	}
	else // command_count > 1 -> Pipeline
	{
		execute_pipeline(cmd_list, data);
	}
	return (data->last_exit_status);
}
