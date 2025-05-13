/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_cmd_table.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:23:07 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/13 16:44:23 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

char	**ft_add_to_argv(char **old_argv, char *word)
{
	char	**new_argv;
	size_t	old_len;
	size_t	i;
	char	*word_copy;

	i = 0;
	old_len = ft_viclen(old_argv);
	word_copy = ft_strdup(word);
	if (!word_copy)
		return (NULL);
	new_argv = malloc(sizeof(char *) * (old_len + 2));
	if (!new_argv)
		return (free(word_copy), NULL);
	i = 0;
	while (i < old_len)
	{
		new_argv[i] = old_argv[i];
		i++;
	}
	new_argv[i] = word_copy;
	new_argv[i + 1] = NULL;
	if (old_argv)
		free(old_argv);
	return (new_argv);
}

static void	ft_populate_cmd(t_token **curr_token, t_cmd *cmd)
{
	t_redir	*new_redir;

	while (*curr_token && (*curr_token)->type != TOKEN_PIPE)
	{
		if ((*curr_token)->type == TOKEN_WORD)
			cmd->argv = ft_add_to_argv(cmd->argv, (*curr_token)->value);
		else if ((*curr_token)->type >= TOKEN_REDIR_IN
			&& (*curr_token)->type <= TOKEN_REDIR_HEREDOC)
		{
			if (!(*curr_token)->next)
				break ;
			new_redir = ft_redir_new((*curr_token)->type,
					(*curr_token)->next->value, 
					(*curr_token)->next->exp_in_herdoc);
			ft_redir_add_back(&cmd->redir, new_redir);
			*curr_token = (*curr_token)->next;
		}
		if ((*curr_token))
			*curr_token = (*curr_token)->next;
		else
			break ;
	}
}

t_cmd	*ft_creat_cmd_table(t_token *token)
{
	t_cmd	*head;
	t_cmd	*new_cmd;
	t_token	*curr_token;
	size_t	n_cmd;
	size_t	i;

	head = NULL;
	i = 0;
	n_cmd = ft_count_cmd(token);
	curr_token = token;
	while (i < n_cmd && curr_token)
	{
		new_cmd = ft_cmd_new();
		if (!new_cmd)
			return (ft_cmd_clear(&head), NULL);
		ft_populate_cmd(&curr_token, new_cmd);
		ft_cmd_add_back(&head, new_cmd);
		if (curr_token && curr_token->type == TOKEN_PIPE)
			curr_token = curr_token->next;
		i++;
	}
	return (head);
}
