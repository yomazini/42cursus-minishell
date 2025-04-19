/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:16:43 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/19 11:11:56 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h> //! MBLater

typedef enum s_token_type
{
	TOKEN_WORD, // 0
	TOKEN_PIPE, // 1
	TOKEN_REDIR_IN, // 2 -> < 
	TOKEN_REDIR_OUT, // 3 -> >
	TOKEN_REDIR_APPEND, // 4 -> >>
	TOKEN_REDIR_HEREDOC, // 5 -> >>
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char	*name;
	char	*value;
	int		flag_env;
	struct s_env *next;
}	t_env;

//libft
int		ft_isspace(int c);
int		ft_isquot(int c);
char	*ft_strncpy(char *dest, const char *src, size_t dstsize);
size_t	ft_strlen(const char *string);

//ft_utils_split.c
int		ft_isoperater(int c);
int		ft_isdouble_op(const char *line, int k);
char	**free_all(char **tkn_array, int i);
char	*ft_alloc_token(int token_len, int j, const char *line);
// ft_utils_error.c
int		quote_error(const char *line);
int		pipe_error(const char *line);
int		redir_error(const char *line);

// Tokenize Part
t_token	*ft_tokenize(char *line);
char	**ft_split_tokens(const char *line);
t_token	*ft_token_new(char *value, t_token_type type);
void	ft_token_add_back(t_token **token_list, t_token *new_node);
void	ft_token_delone(t_token *node);
void	ft_token_clear(t_token **token_list);
int		ft_token_size(t_token *head); // might need later
void	ft_print_token_list(t_token *head); // just for printing
int		ft_synax_error_free(const char *line);

//	expander/env
t_env	*ft_getenv(char **env);
char	*ft_strdup(const char *s1);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strchr(const char *s, int c);
t_env	*ft_tenv_new(char *name, char *value, int flag);
void	ft_tenv_add_back(t_env **env_list, t_env *new_node);
t_env	*ft_tenv_last(t_env *env);
void	ft_tenv_delone(t_env *node);
void	ft_tenv_clear(t_env **token_list);

#endif
