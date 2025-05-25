/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:04:05 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/25 17:44:02 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h" 

int	g_global = 0;

bool	parse_and_expand(t_data *data, t_vars *vars)
{
	if (!ft_synax_error_free(vars->line))
	{
		data->last_exit_status = 258;
		return (false);
	}
	vars->tkn_list = ft_tokenize(vars->line);
	if (!vars->tkn_list)
	{
		data->last_exit_status = 0;
		return (false);
	}
	ft_expander(&vars->tkn_list, data);
	return (true);
}

static bool	ft_handle_heredoc_and_excution(t_data *data, t_vars *vars)
{
	int	herdoc_status;

	herdoc_status = process_heredocs(vars->cmd_list, data);
	if (herdoc_status == EXIT_SUCCESS)
	{
		set_parent_wait_signal_handlers(&vars->old_sigint, &vars->old_sigquit);
		execute_commands(vars->cmd_list, data);
		cleanup_all_heredoc_fds(vars->cmd_list);
		restore_signal_handlers(&vars->old_sigint, &vars->old_sigquit);
		return (true);
	}
	else if (herdoc_status == 77)
	{
		cleanup_all_heredoc_fds(vars->cmd_list);
		data->last_exit_status = 2;
	}
	else if (g_global == 2)
	{
		data->last_exit_status = 1;
		g_global = 0;
		set_signal_handlers_prompt();
	}
	return (false);
}

static void	creat_excute_cleanup(t_data *data, t_vars *vars)
{
	vars->cmd_list = ft_creat_cmd_table(vars->tkn_list);
	ft_token_clear(&vars->tkn_list);
	if (vars->cmd_list)
	{
		ft_handle_heredoc_and_excution(data, vars);
		ft_cmd_clear(&vars->cmd_list);
	}
	else
		data->last_exit_status = 2;
}

bool	handle_input_prompt(t_data *data, t_vars *vars)
{
	extern int	g_global;

	if (g_global == 2 || g_global == 3)
	{
		data->last_exit_status = 1;
		g_global = 0;
	}
	vars->line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
	if (!vars->line)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		return (false);
	}
	if (vars->line[0] == '\0')
	{
		free(vars->line);
		vars->line = NULL;
		return (true);
	}
	add_history(vars->line);
	return (true);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	t_vars	sh_vars;

	(void)ac;
	(void)av;
	if (!main_init_shell(&data, &sh_vars, env))
		return (ft_tenv_clear(&data.env_list), 1);
	while (true)
	{
		sh_vars.line = NULL;
		sh_vars.tkn_list = NULL;
		sh_vars.cmd_list = NULL;
		if (!handle_input_prompt(&data, &sh_vars))
			break ;
		if (sh_vars.line == NULL)
			continue ;
		if (parse_and_expand(&data, &sh_vars))
			creat_excute_cleanup(&data, &sh_vars);
		if (sh_vars.line)
			(free(sh_vars.line), sh_vars.line = NULL);
	}
	if (sh_vars.line)
		free(sh_vars.line);
	ft_tenv_clear(&data.env_list);
	rl_clear_history();
	return (data.last_exit_status);
}
