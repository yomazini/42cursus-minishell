/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:46:17 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/16 15:33:33 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	execute_built_ins(t_cmd *test_cmd, t_data *data)
{
	if (!test_cmd || !test_cmd->arguments[0] || !test_cmd->arguments)
		return (-1);
	if (ft_strncmp(test_cmd->arguments[0], "pwd", 4) == 0)
		return (ft_pwd(data));
	if (ft_strncmp(test_cmd->arguments[0], "echo", 5) == 0)
		return (ft_echo(data, test_cmd));
	if (ft_strncmp(test_cmd->arguments[0], "exit", 5) == 0)
		return (ft_exit(test_cmd, data)); 
	// if (ft_strncmp(test_cmd->arguments[0], "env", 3) == 0)
	// 	return ft_env(data);
	// if (ft_strncmp(test_cmd->arguments[0], "cd", 2) == 0)
	// 	return ft_cd(data, test_cmd); 
	// if (ft_strncmp(test_cmd->arguments[0], "export", 6) == 0)
	// 	return ft_export(data, test_cmd); 
	// if (ft_strncmp(test_cmd->arguments[0], "unset", 5) == 0)
	// 	return ft_unset(data, test_cmd);  
	else
		return (-1);
}
