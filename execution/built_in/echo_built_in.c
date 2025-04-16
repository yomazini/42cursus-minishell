/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:46:19 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/15 22:31:03 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static	int	ft_check_n_flag(char *s)
{
	int	i;

	i = 0;
	if (s && s[0] == '-' && s[1] == 'n')
	{
		i = 2;
		while (s[i] == 'n')
			i++;
		return (s[i] == '\0');
	}
	return (0);
}

int	get_printing_start_index(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->arguments[i])
	{
		if (ft_is_only_whitespace(cmd->arguments[i])
			|| ft_check_n_flag(cmd->arguments[i]))
			i++;
		else
			break ;
	}
	if (cmd->arguments[i] == NULL)
		return (i);
	return (i);
}

void	prepare_echo(t_cmd *cmd, int *print_newline_flag, int *start_index)
{
	int	i;

	i = 1;
	*print_newline_flag = TRUE;
	while (cmd->arguments[i])
	{
		if (ft_check_n_flag(cmd->arguments[i])
			|| ft_is_only_whitespace(cmd->arguments[i]))
		{
			if (ft_check_n_flag(cmd->arguments[i]))
				*print_newline_flag = FALSE;
			i++;
		}
		else
			break ;
	}
	*start_index = get_printing_start_index(cmd);
}

int	ft_echo(t_data *data, t_cmd *cmd)
{
	int	flag_print_newline;
	int	start_index;
	int	i;

	if (!cmd || !cmd->arguments || !cmd->arguments[0])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		data->last_exit_status = EXIT_SUCCESS;
		return (EXIT_SUCCESS);
	}
	prepare_echo(cmd, &flag_print_newline, &start_index);
	i = start_index;
	while (cmd->arguments[i])
	{
		ft_putstr_fd(cmd->arguments[i], STDOUT_FILENO);
		if (cmd->arguments[i + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (flag_print_newline == TRUE)
		ft_putchar_fd('\n', STDOUT_FILENO);
	data->last_exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
