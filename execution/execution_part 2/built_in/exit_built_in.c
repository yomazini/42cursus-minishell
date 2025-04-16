/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:22 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/16 16:19:54 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	ft_handle_exit_too_many_args(t_data *data)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	data->last_exit_status = 1;
	return (1);
}

void	ft_handle_exit_numeric_error(t_cmd *cmd, t_data *data)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (cmd->arguments[1])
		ft_putstr_fd(cmd->arguments[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	// TODO: cleanup before exit
	exit(255);
}
int	ft_exit(t_cmd *cmd, t_data *data)
{
	int				argc;
	int				exit_code_arg;
	unsigned char	exit_status;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	argc = 0;
	while (cmd->arguments[argc])
		argc++;
	if (argc == 1)
	{
		exit_status = (unsigned char)data->last_exit_status;
		// TODO: cleanup
		exit(exit_status);
	}
	if (argc == 2)
	{
		if (ft_valid_number(cmd->arguments[1]))
		{
			exit_code_arg = ft_atoi(cmd->arguments[1]);
			exit_status = (unsigned char)exit_code_arg;
			// TODO: cleanup
			exit(exit_status);
		}
		else
		{
			// TODO: cleanup
			ft_handle_exit_numeric_error(cmd, data);
			return (255);
		}
	}
	else
		return (ft_handle_exit_too_many_args(data));
}
