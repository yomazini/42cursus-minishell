/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:46:17 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/21 15:43:22 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	execute_built_ins(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		return (ft_pwd(cmd, data));
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		return (ft_echo(cmd, data));
	if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		return ft_env(cmd, data);
	if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		return (ft_exit(cmd, data)); 
	if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		return ft_cd(cmd, data); 
	if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		return ft_export( cmd, data); 
	if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		return ft_unset(cmd, data);  
	else
		return (-1);
}
