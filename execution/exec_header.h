/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:26:54 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/05 14:14:48 by ymazini          ###   ########.fr       */
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
# include <stdbool.h>
# include <sys/stat.h>    
# include <errno.h>      
# include <signal.h>
# include "../parser/parser.h"

# define TRUE 1
# define FALSE 0

void	update_shell_level(t_data *data);
int		execute_built_ins(t_cmd *cmd, t_data *data);
int		ft_pwd(t_cmd *cmd, t_data *data);
int		ft_echo(t_cmd *cmd, t_data *data);
int		ft_exit(t_cmd *cmd, t_data *data);
int		ft_env(t_cmd *cmd, t_data *data);
int		ft_cd(t_cmd *cmd, t_data *data);
int		ft_export(t_cmd *cmd, t_data *data);
int		ft_unset(t_cmd *cmd, t_data *data);
void	print_export_env(t_env *env_list);
char	**ft_list_to_array(t_env *env_list);
int		process_dir_change(char *target, t_data *data, int print_path_flag);
int		cd_error(char *path, t_data *data, char *old_pwd_to_free);
int		ft_list_setenv(t_env **env_list_head,
			const char *name, const char *value);
int		update_pwd_env_vars(t_data *data, char *old_pwd_path);
int		ft_is_valid_identifier(const char *name);
int		ft_is_only_whitespace(char *str);
void	free_arr(char **arr);
int		ft_valid_number(char *s);
void	free_cmd_struct(t_cmd *cmd);
int		apply_redirections(t_cmd *cmd);
int		ft_list_setenv(t_env **env_list_head,
			const char *name, const char *value);
char	*ft_list_getenv(t_env *env_list, const char *name);
char	**convert_envlist_to_array(t_env *env_list);
int		is_executable(const char *path);
char	*search_path_variable(const char *command_name, t_env *env_list);
char	*find_command_path(const char *command_name, t_env *env_list);
int		redir_error01(char *filename);
int		apply_redirections(t_cmd *cmd);
void	update_last_exit_status(t_data *data, int wait_status);
int		execute_external_command(t_cmd *cmd, t_data *data);
int		save_original_fds(int *original_stdin, int *original_stdout);
int		restore_original_fds(int original_stdin, int original_stdout);
int		execute_commands(t_cmd *cmd_list, t_data *data);
int		execute_pipeline(t_cmd *cmd_list, t_data *data);
int		is_parent_builtin(t_cmd *cmd);
void	execute_command_node(t_cmd *cmd, t_data *data);
void	execute_child_process(t_cmd *cmd, t_data *data, char *path);
void	setup_child_pipes(int prev_pipe_read, int pipe_fd[2], t_cmd *cmd);
int		wait_for_pipeline(int count, pid_t last_pid, t_data *data);
char	*expand_tilde_path(const char *path_arg, t_env *env_list);

#endif