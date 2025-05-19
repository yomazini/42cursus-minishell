/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:22 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/19 14:53:07 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

void	cleanup_shell_resources(t_data *data)
{
	if (data)
	{
		ft_tenv_clear(&data->env_list);
	}
	rl_clear_history();
}

int	ft_handle_exit_too_many_args(t_data *data)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	data->last_exit_status = 1;
	return (1);
}

void	ft_handle_exit_numeric_error(t_cmd *cmd, t_data *data)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (cmd->argv[1])
		ft_putstr_fd(cmd->argv[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	cleanup_shell_resources(data);
	exit(255);
}

static int	handle_one_argument_exit(t_cmd *cmd, t_data *data)
{
	int				exit_code_arg;
	unsigned char	exit_status;

	if (ft_valid_number(cmd->argv[1]))
	{
		exit_code_arg = ft_atoi(cmd->argv[1]);
		exit_status = (unsigned char)exit_code_arg;
		cleanup_shell_resources(data);
		exit(exit_status);
	}
	else
	{
		ft_handle_exit_numeric_error(cmd, data);
		return (255);
	}
}

int	ft_exit(t_cmd *cmd, t_data *data)
{
	int				argc;
	unsigned char	exit_status_to_use;

	argc = 0;
	if (data->print_flag)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	while (cmd->argv[argc])
		argc++;
	if (argc == 1)
	{
		exit_status_to_use = (unsigned char)data->last_exit_status;
		cleanup_shell_resources(data);
		exit(exit_status_to_use);
	}
	if (argc >= 2)
	{
		if (!ft_valid_number(cmd->argv[1]))
			return (ft_handle_exit_numeric_error(cmd, data), 255);
		else if (argc > 2)
			return (ft_handle_exit_too_many_args(data));
		else
			return (handle_one_argument_exit(cmd, data));
	}
	return (EXIT_FAILURE);
}
