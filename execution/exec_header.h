/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:26:54 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/15 16:19:14 by ymazini          ###   ########.fr       */
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
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <sys/stat.h>    
# include <errno.h>      
# include <signal.h>
# include "../parser/parser.h"

# define TRUE 1
# define FALSE 0
# define MAX_HEREDOCS 16

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
int		process_heredocs(t_cmd *cmd_list, t_data *data);
char	*expand_heredoc_line(char *line, t_data *data);
int		handle_heredoc_redir(int heredoc_fd);
void	cleanup_shell_resources(t_data *data);
void	restore_signal_handlers(struct sigaction *old_sigint,
			struct sigaction *old_sigquit);
void	set_signal_handlers_prompt(void);
void	set_signal_handlers_heredoc(void);
void	set_signal_handlers_ignore(void);
void	set_signal_handlers_default(void);
void	sigint_handler_prompt(int signum);
void	sigint_handler_heredoc(int signum);
void	cleanup_all_heredoc_fds(t_cmd *cmd_list);
int		handle_syntax_error_message(char *message,
			t_data *data, int err_code);
int		handle_empty_command_string_error(t_data *data);
void	ft_print_not_found(char *cmd_name, t_data *data);



// At the top of export_built_in.c, or in exec_header.h if needed elsewhere
typedef struct s_export_op
{
	char	*key;
	char	*value_to_process; // Can be value for set or value to append
	char	*original_arg_str; // For error messages
	int		is_append_mode;
	t_data	*shell_dt;
}	t_export_op;


#endif