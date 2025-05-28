/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:30:20 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/28 01:52:34 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static int	handle_non_empty_command(t_cmd *cmd_node, t_data *data)
{
	if (is_parent_builtin(cmd_node))
		execute_built_ins(cmd_node, data);
	else
	{
		execute_external_command(cmd_node, data);
		if (data->last_exit_status == 127
			|| data->last_exit_status == 126
			|| data->last_exit_status == 2)
			redir_r_emty_cmd_wi_redi(cmd_node, data, FALSE);
	}
	return (data->last_exit_status);
}

static	int	handle_single_command(t_cmd *cmd_node, t_data *data)
{
	data->last_exit_status = 0;
	if (cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0][0] == '\0')
	{
		data->last_exit_status = 127;
		return (redir_r_emty_cmd_wi_redi(cmd_node, data, TRUE));
	}
	else if ((!cmd_node->argv || !cmd_node->argv[0]) && cmd_node->redir
		&& !cmd_node->redir->filename[0])
		return (redir_r_emty_cmd_wi_redi(cmd_node, data, FALSE),
			data->last_exit_status = 1);
	else if ((!cmd_node->argv || !cmd_node->argv[0]) && cmd_node->redir
		&& cmd_node->redir->filename)
		return (redir_r_emty_cmd_wi_redi(cmd_node, data, FALSE),
			data->last_exit_status = 0);
	else if (cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0][0])
		return (handle_non_empty_command(cmd_node, data));
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
	{
		data->print_flag = TRUE;
		return (handle_single_command(cmd_list, data));
	}
	else
		execute_pipeline(cmd_list, data);
	return (data->last_exit_status);
}

static	void	assign_fork(pid_t pid, char *path, t_data *data, t_cmd *cmd)
{
	if (pid < 0)
	{
		perror("minishell: fork");
		data->last_exit_status = 1;
		return ;
	}
	else if (pid == 0)
	{
		set_signal_handlers_default();
		execute_child_process(cmd, data, path);
	}
}

int	execute_external_command(t_cmd *cmd, t_data *data)
{
	char	*path;
	pid_t	pid;
	int		status_dual_job;
	int		err;

	if (cmd->argv[0])
	{
		status_dual_job = print_err_exec_cmd(cmd->argv[0], data);
		if (status_dual_job != 77)
			return (status_dual_job);
	}
	errno = 0;
	path = find_command_path(cmd->argv[0], data->env_list);
	err = errno;
	if (!path)
		return (handle_path_error(cmd->argv[0], err, data));
	pid = fork();
	assign_fork(pid, path, data, cmd);
	(free(path), path = NULL);
	waitpid(pid, &status_dual_job, 0);
	update_last_exit_status(data, status_dual_job);
	return (data->last_exit_status);
}
