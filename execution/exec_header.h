/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_header.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:26:54 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/22 14:48:49 by ymazini          ###   ########.fr       */
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
# include <errno.h>

#include "../parser/parser.h"

struct s_cmd;

typedef struct s_redir
{
	t_token_type	type;
	char			*filename;  // Filename or Delimiter (quote-removed by parser)
	int				heredoc_fd; // FD for heredoc input (set later in exec)
	bool			expand_heredoc; // Flag for heredoc expansion
	struct s_redir	*next;      // Next redirection in the list
}	t_redir;

// Command structure (linked list for pipelines)
typedef struct s_cmd
{
	char			**argv;     // Command and args (quote-removed), NULL terminated.
	struct s_redir	*redirections; // Linked list of redirections for this command.
	struct s_cmd	*next;      // Next command in the pipeline (NULL if last).
}	t_cmd;



# define TRUE 1
# define FALSE 0

// typedef struct t_cmd
// {
// 	char	**arguments;// 
// }	t_cmd;

typedef struct s_data
{
	// char	**envp;
	t_env *env_list;
	int		last_exit_status;
}	t_data;

int		execute_built_ins(t_cmd *cmd, t_data *data); // Use cmd->argv internally
int		ft_pwd(t_cmd *cmd,t_data *data);
int		ft_echo(t_cmd *cmd, t_data *data); // Use cmd->argv internally
int		ft_exit(t_cmd *cmd, t_data *data); // Use cmd->argv internally
int		ft_env(t_cmd *cmd ,t_data *data);
// Signatures for cd/export/unset need checking if they use cmd
int		ft_cd(t_cmd *cmd, t_data *data);
int		ft_export(t_cmd *cmd, t_data *data); // Changed signature
int		ft_unset(t_cmd *cmd, t_data *data); // Changed signature


int	ft_is_valid_identifier(const char *name);

int		ft_is_only_whitespace(char *str);
void	free_arr(char **arr);
int ft_valid_number(char *s);


// --- Temporary Testing / Conversion Prototypes ---
t_cmd	*convert_simple_tokens_to_cmd(t_token *token_list);
void	free_cmd_struct(t_cmd *cmd);
int		is_simple_builtin_command(t_token *token_list); // Helper for main



int ft_list_setenv(t_env **env_list_head, const char *name, const char *value);


#endif