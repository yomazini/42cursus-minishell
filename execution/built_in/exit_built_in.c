/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:22 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/09 20:47:42 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

// Create a new cleanup function (e.g., in tools or main)
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
	ft_putstr_fd("minishell: exit: too many argv\n", STDERR_FILENO);
	data->last_exit_status = 1;
	return (data->last_exit_status);
}

void	ft_handle_exit_numeric_error(t_cmd *cmd, t_data *data)
{
	(void)data;
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (cmd->argv[1])
		ft_putstr_fd(cmd->argv[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		// TODO: cleanup before exit
		cleanup_shell_resources(data);
	exit(255);
}

static	int	handle_two_argument(t_cmd *cmd, t_data *data)
{
	int	exit_code_arg;
	int	exit_status;

	if (ft_valid_number(cmd->argv[1]))
	{
		exit_code_arg = ft_atoi(cmd->argv[1]);
		exit_status = (unsigned char)exit_code_arg;
		// TODO: cleanup
		cleanup_shell_resources(data);
		exit(exit_status);
	}
	else
	{
		// TODO: cleanup
		cleanup_shell_resources(data);
		ft_handle_exit_numeric_error(cmd, data);
		return (255);
	}
}

int	ft_exit(t_cmd *cmd, t_data *data)
{
	int	argc;
	int	exit_status;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	argc = 0;
	while (cmd->argv[argc])
		argc++;
	if (argc == 1)
	{
		exit_status = (unsigned char)data->last_exit_status;
		cleanup_shell_resources(data);
		// TODO: cleanup
		exit(exit_status);
	}
	if (argc == 2)
		return (handle_two_argument(cmd, data));
	else
		return (ft_handle_exit_too_many_args(data));
}
