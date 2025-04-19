/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:46:17 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/19 13:59:48 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	execute_built_ins(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->argv[0] || !cmd->argv)
		return (-1);
	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		return (ft_pwd(data));
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		return (ft_echo(cmd, data));
	if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		return (ft_exit(cmd, data)); 
	// if (ft_strncmp(cmd->argv[0], "env", 3) == 0)
	// 	return ft_env(data);
	// if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
	// 	return ft_cd(data, cmd); 
	// if (ft_strncmp(cmd->argv[0], "export", 6) == 0)
	// 	return ft_export(data, cmd); 
	// if (ft_strncmp(cmd->argv[0], "unset", 5) == 0)
	// 	return ft_unset(data, cmd);  
	else
		return (-1);
}
