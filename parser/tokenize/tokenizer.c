/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:19:16 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/16 18:13:36 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND
		|| type == TOKEN_REDIR_HEREDOC);
}

t_token_type	ft_token_type(char *token)
{
	if (strcmp(token, "|") == 0)
		return (TOKEN_PIPE);
	else if (strcmp(token, "<") == 0)
		return (TOKEN_REDIR_IN);
	else if (strcmp(token, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (strcmp(token, ">>") == 0)
		return (TOKEN_REDIR_APPEND);
	else if (strcmp(token, "<<") == 0)
		return (TOKEN_REDIR_HEREDOC);
	else
		return (TOKEN_WORD);
}

int	ft_synatx_error(t_token *head)
{
	t_token	*current;

	current = head;
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (current->next == NULL)
				break ;
			else if (is_redirection(current->next->type))
			{
				printf("\033[1;31mSyntax error:\033[0m "
					"\033[1;36mconsecutive redirection operators\033[0m\n");
				return (1);
			}
			else if (current->next->type == TOKEN_PIPE)
			{
				printf("\033[1;31mSyntax error:\033[0m "
					"\033[1;36mnear unexpected token '|'\033[0m\n");
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

static t_token	*ft_build_token_list(char **tkn_array)
{
	int				i;
	t_token			*head;
	t_token			*new_node;
	t_token_type	type;

	i = 0;
	head = NULL;
	new_node = NULL;
	while (tkn_array[i])
	{
		type = ft_token_type(tkn_array[i]);
		new_node = ft_token_new(tkn_array[i], type);
		if (!new_node)
			return (ft_token_clear(&head), NULL);
		ft_token_add_back(&head, new_node);
		i++;
	}
	return (head);
}

t_token	*ft_tokenize(char *line)
{
	char			**tkn_array;
	t_token			*head;

	tkn_array = NULL;
	head = NULL;
	if (!line)
		return (NULL);
	tkn_array = ft_split_tokens(line);
	if (!tkn_array)
		return (NULL);
	head = ft_build_token_list(tkn_array);
	free(tkn_array);
	tkn_array = NULL;
	if (ft_synatx_error(head))
		return (ft_token_clear(&head), NULL);
	return (head);
}
