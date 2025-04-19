/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_converter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:02:10 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/19 14:03:28 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: this is temp file; untill mehdi give me the ready t_cmd struct; {only one and simple command --> {NO pipe and No Redirect } }

#include "../exec_header.h" // Includes t_token, t_cmd, t_redir defs

// Counts only WORD tokens in a simple list (no pipes/redirects handled here)
static int	count_word_tokens_simple(t_token *token_list)
{
	int		count = 0;
	t_token	*current = token_list;

	while (current)
	{
		// For this simple converter, assume all WORDs are arguments
		if (current->type == TOKEN_WORD)
			count++;
		// Ignore PIPE/REDIR tokens for now
		current = current->next;
	}
	return (count);
}

// Creates argv array from WORD tokens in a simple list
static char	**create_argv_from_simple_tokens(t_token *token_list, int word_count)
{
	char	**argv_array;
	t_token	*current = token_list;
	int		i = 0;

	argv_array = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!argv_array)
		return (NULL);

	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			argv_array[i] = ft_strdup(current->value);
			if (!argv_array[i])
			{
				free_arr(argv_array); // Frees partially allocated array
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	argv_array[i] = NULL; // Null-terminate
	return (argv_array);
}

// Creates a single t_cmd from a token list *IF* it's simple
// Returns NULL if list contains pipes/redirects or on error
t_cmd	*convert_simple_tokens_to_cmd(t_token *token_list)
{
	t_cmd	*new_cmd = NULL;
	int		word_count = 0;

	if (!token_list)
		return (NULL);

	// Create the command struct instance
	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);

	// Initialize fields for a single command node
	new_cmd->redirections = NULL; // No redirection handling in this simple version
	new_cmd->next = NULL;         // No pipe handling in this simple version

	// Create the argv array
	word_count = count_word_tokens_simple(token_list);
	new_cmd->argv = create_argv_from_simple_tokens(token_list, word_count);
	if (!new_cmd->argv)
	{
		free(new_cmd); // Cleanup if argv allocation fails
		return (NULL);
	}

	return (new_cmd);
}

// Frees the t_cmd struct created by the simple converter
void	free_cmd_struct(t_cmd *cmd)
{
	if (!cmd)
		return;
	free_arr(cmd->argv);
	// free(cmd->redirections); // Add when redirections implemented
	free(cmd);
}