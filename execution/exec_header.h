/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:26:54 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/16 14:16:14 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_HEADER_H
# define EXEC_HEADER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# define TRUE 1
# define FALSE 0

typedef struct t_cmd
{
	char	**arguments;// 
}	t_cmd;

typedef struct s_data
{
	char	**envp;
	int		last_exit_status;
}	t_data;

int		execute_built_ins(t_cmd *test_cmd, t_data *data);
int		ft_pwd(t_data *data);
int		ft_echo(t_data *data, t_cmd *cmd);
int		ft_exit(t_cmd *cmd, t_data *data);
int		ft_env(t_data *data);
int		ft_cd(t_cmd *cmd, t_data *data);
int		ft_export(t_data *data, t_cmd *cmd);
int		ft_unset(t_data *data, t_cmd *cmd);
int		ft_is_only_whitespace(char *str);
void	free_arr(char **arr);
int ft_valid_number(char *s);

#endif