/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_initialisation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:46:59 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/24 13:08:39 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	main_init_shell(t_data *data, t_vars *vars, char **env)
{
	vars->line = NULL;
	vars->tkn_list = NULL;
	vars->cmd_list = NULL;
	data->last_exit_status = EXIT_SUCCESS;
	data->env_list = ft_getenv(env);
	if (!data->env_list && env && env[0])
	{
		ft_putstr_fd("minishell: Warning: env list init failed.\n", 2);
		return (false);
	}
	update_shell_level(data);
	rl_catch_signals = 0;
	set_signal_handlers_prompt();
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (false);
	return (true);
}
