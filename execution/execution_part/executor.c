/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:30:20 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/19 23:32:19 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int handle_empty_command_string_error1(t_data *data, char *cmd_name_for_error)
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
static int handle_redir_or_empty_cmd_with_redir(t_cmd *cmd_node, t_data *data, int is_empty_str_cmd)
{
	if ((cmd_node && cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0] && cmd_node->argv[0][0] && cmd_node->redir) && ( cmd_node->redir->type == TOKEN_REDIR_IN || cmd_node->redir->type == TOKEN_REDIR_HEREDOC))
		return (0);

	pid_t pid;
	int  child_status = 0;
	pid = fork();
	if (pid < 0)
	{
		data->last_exit_status = EXIT_FAILURE;
		perror("minishell: fork for redirection");
		return (EXIT_FAILURE);
	}
	if (pid == 0) // Child
	{
		set_signal_handlers_default();
		if (apply_redirections(cmd_node) != 0)
			exit(EXIT_FAILURE); // Redirection failed
		if (is_empty_str_cmd)	// If it was "" > file
		{
			handle_empty_command_string_error1(data, cmd_node->argv[0]); // Prints error
			exit(127);													 // "" is command not found
		}
		exit(EXIT_SUCCESS); // Only redirections, success
	}
	// Parent
	waitpid(pid, &child_status, 0);
	if ((cmd_node && !cmd_node->argv && cmd_node->redir) &&  cmd_node->redir->type == TOKEN_REDIR_IN)
		update_last_exit_status(data, child_status);
	return (data->last_exit_status);
}
// static int handle_redir_or_empty_cmd(t_cmd *cmd_node, t_data *data)
// {
// 	pid_t pid;
// 	int child_status;
// 	int cmd_is_empty_str;

// 	cmd_is_empty_str = (cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0][0] == '\0');

// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("minishell: fork");
// 		return (data->last_exit_status = EXIT_FAILURE, EXIT_FAILURE);
// 	}
// 	if (pid == 0) // Child
// 	{
// 		set_signal_handlers_default();
// 		if (apply_redirections(cmd_node) != 0) // Apply redirections
// 			exit(EXIT_FAILURE);				   // Exit if redirection itself failed
// 		if (!cmd_node->argv || !cmd_node->argv[0] || cmd_is_empty_str)
// 		{
// 			// If no command, or command is "", it's a failure after redirection
// 			if (cmd_is_empty_str)
// 				handle_empty_command_string_error1(data, cmd_node->argv[0]); // Prints error
// 			// else: No command was given, redirections done. Bash exits 0 here.
// 			// For consistency with how "" > file behaves, we can exit 0 if not empty string.
// 			exit(cmd_is_empty_str ? 127 : EXIT_SUCCESS);
// 		}
// 		// Should not be reached if logic above is correct for these cases
// 		exit(EXIT_FAILURE); // Fallback
// 	}
// 	// Parent
// 	waitpid(pid, &child_status, 0);
// 	update_last_exit_status(data, child_status);
// 	return (data->last_exit_status);
// }

static int handle_single_command(t_cmd *cmd_node, t_data *data)
{
	// Case 1: Command word is "" (empty string, e.g., "$EMPTYVAR" > file)
	if (cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0][0] == '\0')
	{
		// Treat like redirection-only but with a command-not-found error at the end.
		// The redirections should still be processed.
		data->last_exit_status = 127;
		return (handle_redir_or_empty_cmd_with_redir(cmd_node, data, TRUE));
	}
	// Case 2: No command word AT ALL, but redirections exist (e.g., "> file", "$UNSETVAR > file")
	else if ((!cmd_node->argv || !cmd_node->argv[0]) && cmd_node->redir)
	{
		return (handle_redir_or_empty_cmd_with_redir(cmd_node, data, FALSE));
	}
	// Case 3: Command word is present AND non-empty
	else if (cmd_node->argv && cmd_node->argv[0] && cmd_node->argv[0][0] != '\0')
	{
		if (is_parent_builtin(cmd_node)) // cd, export <args>, unset, exit
			execute_built_ins(cmd_node, data);
		else
		// External command OR child-safe builtin (echo, pwd, env, export no-args)
		{
			execute_external_command(cmd_node, data);
			handle_redir_or_empty_cmd_with_redir(cmd_node, data, FALSE);
		}
	}
	// Case 4: Invalid state (e.g., no command word AND no redirections) - Parser should prevent.
	// ==> deleted
	return (data->last_exit_status);
}

int execute_commands(t_cmd *cmd_list, t_data *data)
{
	int command_count;
	t_cmd *counter;

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

int execute_external_command(t_cmd *cmd, t_data *data)
{
	char *executable_path;
	pid_t child_pid;
	int wait_status;
	int original_errno_find_path;

	// --- Handle special cases for ., .., ./, ../ as command name ---
	if (cmd->argv[0])
	{
		if (ft_strncmp(cmd->argv[0], ".", 2) == 0)
		{
			ft_putstr_fd("minishell: .: filename argument required\n", STDERR_FILENO);
			ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
			return (data->last_exit_status = 2, 2);
		}
		if (ft_strncmp(cmd->argv[0], "..", 3) == 0)
		{
			ft_putstr_fd("minishell: ..: command not found\n", STDERR_FILENO);
			return (data->last_exit_status = 127, 127);
		}
		// For "./" or "../" or any path containing '/', find_command_path will use it directly
		// and is_executable will set errno to EISDIR if it's a directory.
	}
	// --- End special handling ---

	errno = 0; // Clear errno before calling find_command_path
	executable_path = find_command_path(cmd->argv[0], data->env_list);
	original_errno_find_path = errno; // Save errno immediately

	if (!executable_path) // Path not found OR path is invalid (e.g. directory, no perms)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		if (original_errno_find_path == EISDIR)
		{
			ft_putstr_fd(": is a directory\n", STDERR_FILENO);
			data->last_exit_status = 126;
			return (126);
		}
		else if (original_errno_find_path == EACCES)
		{
			ft_putstr_fd(": Permission denied--> this is not printing\n", STDERR_FILENO);
			data->last_exit_status = 126;
			return (126);
		}
		else
		{ // Default "command not found" for ENOENT or other find_path errors
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			data->last_exit_status = 127;
			return (127);
		}
	}
	// ... (fork, child process with execute_child_process, parent waitpid - no changes here) ...
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
