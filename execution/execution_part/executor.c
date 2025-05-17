/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:30:20 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/17 22:16:16 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static int	handle_redirection_only_command(t_cmd *cmd_node, t_data *data)
{
	pid_t	pid;
	int		child_status;

	pid = fork();
	if (pid < 0)
	{
		data->last_exit_status = EXIT_FAILURE;
		perror("minishell: fork for redirection");
		return (EXIT_FAILURE);
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

static int	handle_single_command(t_cmd *cmd_node, t_data *data)
{
	if (cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0][0])
	{
		if (is_parent_builtin(cmd_node))
			execute_built_ins(cmd_node, data);
		else
			execute_external_command(cmd_node, data);
		return (data->last_exit_status);
	}
	else if ((!cmd_node->argv || !cmd_node->argv[0] || !cmd_node->argv[0][0])
			&& cmd_node->redir)
	{
		return (handle_redirection_only_command(cmd_node, data));
	}
	else if (cmd_node->argv && cmd_node->argv[0] && !cmd_node->argv[0][0])
	{
		return (handle_empty_command_string_error(data));
	}
	else if (cmd_node->argv)
	{
		data->last_exit_status = 258;
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		return (data->last_exit_status);
	}
	return(data->last_exit_status);
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
	{
		if (!cmd_list->argv || !cmd_list->argv[0])
		{
			return (handle_syntax_error_message("syntax error \
				near unexpected token `|'", data, 258));
		}
		execute_pipeline(cmd_list, data);
	}
	return (data->last_exit_status);
}

int	execute_external_command(t_cmd *cmd, t_data *data)
{
	char	*executable_path;
	pid_t	child_pid;
	int		wait_status;
	int original_errno;
	// --- Handle '.' and '..' specially ---
	if (cmd->argv[0] && ft_strncmp(cmd->argv[0], ".", 2) == 0)
	{
		ft_putstr_fd("minishell: .: filename argument required\n", STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO); // Mimic bash
		data->last_exit_status = 2; // Common error code for builtin usage
		return (2);
	}
	if (cmd->argv[0] && ft_strncmp(cmd->argv[0], "..", 3) == 0)
	{
		ft_putstr_fd("minishell: ..: command not found\n", STDERR_FILENO);
		data->last_exit_status = 127;
		return (127);
	}
	// --- End special handling ---


	errno = 0; // Clear errno before calling find_command_path
	executable_path = find_command_path(cmd->argv[0], data->env_list);
	original_errno = errno; // Save errno immediately after find_command_path

	if (!executable_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		if (original_errno == EISDIR) // Check if find_path failed because it was a directory
		{
			ft_putstr_fd(": is a directory\n", STDERR_FILENO);
			data->last_exit_status = 126;
			return (126);
		}
		else if (original_errno == EACCES) // Check for permission denied on a file path
		{
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			data->last_exit_status = 126;
			return (126);
		}
		else // Default "command not found"
		{
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			data->last_exit_status = 127;
			return (127);
		}
	}

	// executable_path = find_command_path(cmd->argv[0], data->env_list);
	// executable_path = find_command_path(cmd->argv[0], data->env_list);
	// if (!executable_path)
	// 	return (ft_print_not_found(cmd->argv[0], data), 127);
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
