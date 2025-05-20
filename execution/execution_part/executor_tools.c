/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:14:55 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/20 18:00:27 by ymazini          ###   ########.fr       */
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

int	print_err_exec_cmd(char *cmd, t_data *data)
{
	if (ft_strncmp(cmd, ".", 2) == 0)
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		return (data->last_exit_status = 2, 2);
	}
	if (ft_strncmp(cmd, "..", 3) == 0)
	{
		ft_putstr_fd("minishell: ..: command not found\n", 2);
		return (data->last_exit_status = 127, 127);
	}
	return (77);
}

int	handle_path_error(char *name, int err, t_data *data)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	if (err == EISDIR)
	{
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		return (data->last_exit_status = 126, 126);
	}
	else if (err == EACCES)
	{
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		return (data->last_exit_status = 126, 126);
	}
	else
	{
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		return (data->last_exit_status = 127, 127);
	}
}

int	redir_r_emty_cmd_wi_redi(t_cmd *cmd, t_data *data, int is_empty)
{
	pid_t	child;
	int		status;

	if (cmd && cmd->argv && cmd->argv[0] && cmd->redir
		&& (cmd->redir->type == TOKEN_REDIR_IN
			|| cmd->redir->type == TOKEN_REDIR_HEREDOC))
		return (0);
	child = fork();
	if (child < 0)
		return (perror("minishell: fork for redir"),
			data->last_exit_status = 1, 1);
	if (child == 0)
	{
		set_signal_handlers_default();
		if (apply_redirections(cmd) != 0)
			exit(EXIT_FAILURE);
		if (is_empty)
			handle_empty_command_string_error1(data, cmd->argv[0]);
		exit(data->last_exit_status);
	}
	waitpid(child, &status, 0);
	if (cmd && !cmd->argv && cmd->redir)
		update_last_exit_status(data, status);
	return (data->last_exit_status);
}
