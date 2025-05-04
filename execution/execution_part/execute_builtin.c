/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:46:17 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/04 15:33:45 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

// int	execute_built_ins(t_cmd *cmd, t_data *data)
// {
// 	int	builtin_status;
// 	int	original_stdin;
// 	int	original_stdout;
// 	int	redir_status;

// 	original_stdin = -1;
// 	original_stdout = -1;
// 	builtin_status = -1;

// 	if (!cmd || !cmd->argv || !cmd->argv[0])
// 		return (-1);
// 	if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
// 		return (ft_exit(cmd, data));
// 	if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
// 		return (ft_cd(cmd, data));
// 	if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
// 		return (ft_unset(cmd, data));
// 	if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
// 	{
// 		if (cmd->argv[1] == NULL)
// 		{
// 			if (save_original_fds(&original_stdin, &original_stdout) == -1)
// 				return (EXIT_FAILURE);
// 			redir_status = apply_redirections(cmd);
// 			if (redir_status == 0)
// 				builtin_status = ft_export(cmd, data);
// 			else
// 				data->last_exit_status = EXIT_FAILURE;
// 			restore_original_fds(original_stdin, original_stdout);
// 			return (data->last_exit_status);
// 		}
// 		else
// 		{
// 			return (ft_export(cmd, data));
// 		}
// 	}
// 	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0 ||
// 		ft_strncmp(cmd->argv[0], "echo", 5) == 0 ||
// 		ft_strncmp(cmd->argv[0], "env", 4) == 0)
// 	{
// 		if (save_original_fds(&original_stdin, &original_stdout) == -1)
// 			return (EXIT_FAILURE);

// 		redir_status = apply_redirections(cmd);
// 		if (redir_status == 0)
// 		{
// 			if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
// 				builtin_status = ft_pwd(cmd, data);
// 			else if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
// 				builtin_status = ft_echo(cmd, data);
// 			else if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
// 				builtin_status = ft_env(cmd, data); 
// 		}
// 		else
// 		{
// 			data->last_exit_status = EXIT_FAILURE;
// 			builtin_status = EXIT_FAILURE;
// 		}
// 		restore_original_fds(original_stdin, original_stdout);
// 		return (builtin_status);
// 	}
// 	return (-1);
// }

/* ---- execute_builtin refactoring ---- */

/* Handle export builtin with no arguments */
static int	handle_export_display(t_cmd *cmd, t_data *data)
{
	int	original_stdin;
	int	original_stdout;
	int	redir_status;
	int	builtin_status;

	if (save_original_fds(&original_stdin, &original_stdout) == -1)
		return (EXIT_FAILURE);
	redir_status = apply_redirections(cmd);
	if (redir_status == 0)
		builtin_status = ft_export(cmd, data);
	else
		data->last_exit_status = EXIT_FAILURE;
	restore_original_fds(original_stdin, original_stdout);
	return (data->last_exit_status);
}

/* Handle output builtins (pwd, echo, env) */
static int	handle_output_builtin(t_cmd *cmd, t_data *data)
{
	int	original_stdin;
	int	original_stdout;
	int	redir_status;
	int	builtin_status;

	builtin_status = -1;
	if (save_original_fds(&original_stdin, &original_stdout) == -1)
		return (EXIT_FAILURE);
	redir_status = apply_redirections(cmd);
	if (redir_status == 0)
	{
		if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
			builtin_status = ft_pwd(cmd, data);
		else if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
			builtin_status = ft_echo(cmd, data);
		else if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
			builtin_status = ft_env(cmd, data);
	}
	else
	{
		data->last_exit_status = EXIT_FAILURE;
		builtin_status = EXIT_FAILURE;
	}
	restore_original_fds(original_stdin, original_stdout);
	return (builtin_status);
}

/* Main builtin executor */
int	execute_built_ins(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		return (ft_exit(cmd, data));
	if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		return (ft_cd(cmd, data));
	if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		return (ft_unset(cmd, data));
	if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
	{
		if (cmd->argv[1] == NULL)
			return (handle_export_display(cmd, data));
		else
			return (ft_export(cmd, data));
	}
	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0
		||ft_strncmp(cmd->argv[0], "echo", 5) == 0
		||ft_strncmp(cmd->argv[0], "env", 4) == 0)
		return (handle_output_builtin(cmd, data));
	return (-1);
}
