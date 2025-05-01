/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:19 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/29 22:38:48 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static void	print_the_env(t_env *current_env_node)
{
	ft_putstr_fd(current_env_node->name, STDOUT_FILENO);
	ft_putchar_fd('=', STDOUT_FILENO);
	ft_putstr_fd(current_env_node->value, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

int	ft_env(t_cmd *cmd, t_data *data)
{
	t_env	*current_env_node;

	if (cmd->argv[1] != NULL)
	{
		ft_putstr_fd("minishell: env: Too many arguments\n", STDERR_FILENO);
		data->last_exit_status = 127;
		return (127);
	}
	current_env_node = data->env_list;
	if (!current_env_node)
	{
		data->last_exit_status = EXIT_SUCCESS;
		return (EXIT_SUCCESS);
	}
	while (current_env_node != NULL)
	{
		if (current_env_node->name != NULL && current_env_node->value != NULL)
		{
			print_the_env(current_env_node);
		}
		current_env_node = current_env_node->next;
	}
	data->last_exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
