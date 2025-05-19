/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:30:20 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/19 15:39:24 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	handle_empty_command_string_error1(t_data *data, char *cmd_name_for_error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd_name_for_error && cmd_name_for_error[0] == '\0')
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	else if (cmd_name_for_error)
	{	
		ft_putstr_fd(cmd_name_for_error, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("command not found\n", STDERR_FILENO);
	data->last_exit_status = 127;
	return (127);
}

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
		set_signal_handlers_default();
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
	if (cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0][0] != '\0')
	{
		if (is_parent_builtin(cmd_node))
			execute_built_ins(cmd_node, data);
		else
			execute_external_command(cmd_node, data);
	}
	else if (cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0][0] == '\0')
	{
		// For "" > file, Bash creates the file then says "" command not found.
		// We can achieve this by trying to execute it, which will handle redirections
		// in a child and then fail on execve("").
		execute_external_command(cmd_node, data);
	}
	// Case 3: No command word AT ALL (argv is NULL or argv[0] is NULL), but redirections exist
	else if ((!cmd_node->argv || !cmd_node->argv[0]) && cmd_node->redir)
	{
		return (handle_redirection_only_command(cmd_node, data));
	}
	// Case 4: Invalid state - no command, no redirections (parser should prevent this)
	// Or other syntax errors not caught by parser.
	// else
	// {
	// 	data->last_exit_status = 2; // General syntax-like error for malformed cmd_node
	// 	ft_putstr_fd("minishell: invalid command structure\n", STDERR_FILENO);
	// 	return (data->last_exit_status);
	// }
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
	{
		if (!cmd_list->argv || !cmd_list->argv[0] || cmd_list->argv[0][0] == '\0')
		{
			// If first cmd is empty string or NULL, it's a syntax error for pipe
			// (e.g., "| ls" or "$EMPTYVAR | ls")
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
			data->last_exit_status = 258; // Bash syntax error for pipe
			return (data->last_exit_status);
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
	int original_errno_find_path;
	

	// --- Handle command that is an empty string "" ---
	if (cmd->argv[0] && cmd->argv[0][0] == '\0')
	{
		// For "" > file, bash creates file then errors.
		// We simulate this by forking, applying redirections, then failing.
		child_pid = fork();
		if (child_pid < 0) { perror("minishell: fork"); data->last_exit_status = 1; return 1; }
		if (child_pid == 0) 
		{ // Child
			set_signal_handlers_default();
			if (apply_redirections(cmd) != 0) exit(EXIT_FAILURE);
			handle_empty_command_string_error1(data, cmd->argv[0]); // Prints error
			exit(127); // "" is command not found
		}
		waitpid(child_pid, &wait_status, 0);
		update_last_exit_status(data, wait_status);
		return (data->last_exit_status);
	}
	// --- End handling empty string command ---



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
	original_errno_find_path = errno; // Save errno immediately after find_command_path

	if (!executable_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		if (original_errno_find_path == EISDIR) // Check if find_path failed because it was a directory
		{
			ft_putstr_fd(": is a directory\n", STDERR_FILENO);
			data->last_exit_status = 126;
			return (126);
		}
		else if (original_errno_find_path == EACCES) // Check for permission denied on a file path
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
