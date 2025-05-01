/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:39:25 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/01 14:52:10 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../exec_header.h"

static int	str_is_positive_numeric(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	update_shell_level(t_data *data)
{
	char	*current_shlvl_str;
	int		current_level;
	int		new_level;
	char	*new_shlvl_str;

	current_level = 0;
	new_level = 1;

	current_shlvl_str = ft_list_getenv(data->env_list, "SHLVL");

	if (current_shlvl_str)
	{
		if (str_is_positive_numeric(current_shlvl_str))
		{
			current_level = ft_atoi(current_shlvl_str);
			if (current_level < 0)
				current_level = 0;
		}
		if (current_level >= 999)
		{
			ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
			ft_putnbr_fd(current_level + 1, STDERR_FILENO);
			ft_putstr_fd(") too high, resetting to 1\n", STDERR_FILENO);
			new_level = 1;
		}
		else
			new_level = current_level + 1;
	}
	new_shlvl_str = ft_itoa(new_level);
	if (!new_shlvl_str)
	{
		perror("minishell: ft_itoa failed for SHLVL");
		return ;
	}

	if (ft_list_setenv(&data->env_list, "SHLVL", new_shlvl_str) == -1)
		perror("minishell: ft_list_setenv failed for SHLVL");
	free(new_shlvl_str);
}
