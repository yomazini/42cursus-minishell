/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:09:03 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/04 18:27:28 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

void	update_last_exit_status(t_data *data, int wait_status)
{
	int	term_sig;

	if (WIFEXITED(wait_status))
	{
		data->last_exit_status = WEXITSTATUS(wait_status);
	}
	else if (WIFSIGNALED(wait_status))
	{
		term_sig = WTERMSIG(wait_status);
		data->last_exit_status = 128 + term_sig;
		if (term_sig == SIGQUIT)
			ft_putstr_fd("Quit: 3/n", STDERR_FILENO);
		else if (term_sig == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
	}
	else
		data->last_exit_status = EXIT_FAILURE;
}

int	ft_is_valid_identifier(const char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
