/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:56:25 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/20 14:13:16 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	handle_syntax_error_message(char *message,
		t_data *data, int err_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	data->last_exit_status = err_code;
	return (err_code);
}

int	handle_empty_command_string_error(t_data *data)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	data->last_exit_status = 127;
	return (data->last_exit_status);
}

void	ft_print_not_found(char *cmd_name, t_data *data)
{
	data->last_exit_status = 127;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}
