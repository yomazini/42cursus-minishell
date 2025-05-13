/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:30:20 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/13 15:18:48 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static int	handle_syntax_error_message(char *message,
		t_data *data, int err_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	data->last_exit_status = err_code;
	return (err_code);
}

static int	handle_single_command(t_cmd *cmd_node, t_data *data)
{
	int		child_status;
	pid_t	pid;

	if (cmd_node->argv && cmd_node->argv[0])
	{
		if (is_parent_builtin(cmd_node))
			execute_built_ins(cmd_node, data);
		else if ((cmd_node->argv[0][0]) ||
			(!cmd_node->argv[0][0] && !data->empty_arg))
			execute_external_command(cmd_node, data);
		return (data->last_exit_status);
	}
	else if ((!cmd_node->argv || !cmd_node->argv[0] || !cmd_node->argv[0][0])
			&& cmd_node->redir)
	{
		pid = fork();
		if (pid < 0)
		{
			data->last_exit_status = 1;
			return (perror("minishell: fork"), 1);
		}
		if (pid == 0)
		{
			if (apply_redirections(cmd_node) != 0)
				exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		waitpid(pid, &child_status, 0);
		update_last_exit_status(data, child_status);
		return (data->last_exit_status);
	}
	else if (!cmd_node->argv || !cmd_node->argv[0] || !cmd_node->argv[0][0])
	{
		if (cmd_node->argv && cmd_node->argv[0] && !cmd_node->argv[0][0])
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd("\"\"", STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			data->last_exit_status = 127;
		}
		else
			data->last_exit_status = 258;
		return (data->last_exit_status);
	}
	else
		return (handle_syntax_error_message("invalid structure", data, 2));
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
			data->last_exit_status = 258;
			return (data->last_exit_status);
		}
		execute_pipeline(cmd_list, data);
	}
	return (data->last_exit_status);
}

static void	ft_print_not_found(char *cmd_name, t_data *data)
{
	data->last_exit_status = 127;
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
		return (ft_print_not_found(cmd->argv[0], data), 127);
	child_pid = fork();
	if (child_pid < 0)
	{
		data->last_exit_status = EXIT_FAILURE;
		return (perror("minishell: fork"), free(executable_path), EXIT_FAILURE);
	}
	else if (child_pid == 0)
	{
		set_signal_handlers_default();
		execute_child_process(cmd, data, executable_path);
	}
	free(executable_path);
	wait_status = 0;
	waitpid(child_pid, &wait_status, 0);
	update_last_exit_status(data, wait_status);
	return (data->last_exit_status);
}
