/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:30:20 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/04 16:37:56 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static int	handle_invalid_command(t_cmd *cmd_list, t_data *data)
{
	(void)cmd_list;
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'", 2);
	ft_putstr_fd(" (or invalid command)\n", 2);
	data->last_exit_status = 2;
	return (data->last_exit_status);
}

static int	handle_single_command(t_cmd *cmd_list, t_data *data)
{
	if (!cmd_list->argv || !cmd_list->argv[0])
		return (handle_invalid_command(cmd_list, data));
	if (is_parent_builtin(cmd_list))
		execute_built_ins(cmd_list, data);
	else
		execute_external_command(cmd_list, data);
	return (data->last_exit_status);
}

int	execute_commands(t_cmd *cmd_list, t_data *data)
{
	int		command_count;
	t_cmd	*counter;

	if (!cmd_list)
		return (data->last_exit_status = 0, 0);
	command_count = 0;
	counter = cmd_list;
	while (counter != NULL)
	{
		command_count++;
		counter = counter->next;
	}
	if (command_count == 1)
		return (handle_single_command(cmd_list, data));
	else
	{
		if (!cmd_list->argv || !cmd_list->argv[0])
		{
			ft_putstr_fd("mini: syntax error near unexpected token`|'\n", 2);
			data->last_exit_status = 2;
			return (data->last_exit_status);
		}
		execute_pipeline(cmd_list, data);
	}
	return (data->last_exit_status);
}

void	ft_print_not_found(char *cmd_name)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(" : command not found\n", STDERR_FILENO);
}

int	execute_external_command(t_cmd *cmd, t_data *data)
{
	char	*executable_path;
	pid_t	child_pid;
	int		wait_status;

	executable_path = find_command_path(cmd->argv[0], data->env_list);
	if (!executable_path)
	{
		ft_print_not_found(cmd->argv[0]);
		data->last_exit_status = 127;
		return (127);
	}
	child_pid = fork();
	if (child_pid < 0)
	{
		data->last_exit_status = EXIT_FAILURE;
		return (perror("minishell: fork"), free(executable_path), EXIT_FAILURE);
	}
	else if (child_pid == 0)
		execute_child_process(cmd, data, executable_path);
	free(executable_path);
	wait_status = 0;
	waitpid(child_pid, &wait_status, 0);
	update_last_exit_status(data, wait_status);
	return (data->last_exit_status);
}
