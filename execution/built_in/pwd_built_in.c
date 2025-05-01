/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_built_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:10:19 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/29 22:38:24 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	ft_pwd(t_cmd *cmd, t_data *data)
{
	char	*pwd;

	if (cmd->argv[1])
	{
		ft_putstr_fd("pwd: too many arguments\n", STDERR_FILENO);
		data->last_exit_status = EXIT_FAILURE;
		return (data->last_exit_status);
	}
	pwd = NULL;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd function failed: ");
		data->last_exit_status = EXIT_FAILURE;
		return (free(pwd), pwd = NULL, EXIT_FAILURE);
	}
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(pwd);
	pwd = NULL;
	data->last_exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
