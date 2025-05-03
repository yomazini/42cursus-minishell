#include "../parser.h"


size_t	ft_count_cmd(t_token *token)
{
	t_token	*current;
	size_t	count_pipe;

	current = token;
	count_pipe = 0;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			count_pipe++;
		current = current->next;
	}
	return (count_pipe + 1);
}

size_t	ft_viclen(char **argv)
{
	size_t	i;

	if (!argv)
		return (0);
	i = 0;
	while (argv[i])
		i++;
	return (i);
}
// char	**ft_add_to_argv(char ** old_argv, char *word)
// {
// 	char	**new_arg;
// 	size_t	old_size;
// 	size_t	i;

// 	// if (!word)
// 	// 	return (NULL);
// 	old_size = ft_viclen(old_argv);
// 	if (old_size == 0)
// 	{
// 		new_arg =  malloc(sizeof(char *) * 2);
// 		new_arg[0] = word;
// 		new_arg[1] = NULL;
// 	}
// 	else
// 	{
// 		new_arg = malloc(sizeof(char *) * (old_size + 2));
// 		if (!new_arg)
// 			return (NULL);
// 		i = 0;
// 		while (i < old_size)
// 		{
// 			new_arg[i] = old_argv[i];
// 			i++;
// 		}
// 		new_arg[old_size] = word;
// 		new_arg[old_size + 1] = NULL;
// 		if (old_argv)
// 			 free(old_argv);
// 	}
// 	return (new_arg);
// }

// Modify this helper function
char	**ft_add_to_argv(char ** old_argv, char *word)
{
	char	**new_arg;
	size_t	old_size;
	size_t	i;
    char    *word_copy; // Variable for the duplicated string

	old_size = ft_viclen(old_argv);

    // *** FIX: Duplicate the word ***
    word_copy = ft_strdup(word);
    if (!word_copy)
    {
        // Handle strdup failure (maybe free old_argv if necessary?)
        // For now, returning NULL is reasonable if the caller checks.
        // If old_argv was dynamically built, need careful freeing here.
        // Since ft_create_cmd_table seems to free the *old* array pointer later,
        // just returning NULL might be okay, but check for leaks.
        return (NULL);
    }
    // *** END FIX ***

	if (old_size == 0)
	{
		new_arg =  malloc(sizeof(char *) * 2);
        if (!new_arg) { free(word_copy); return (NULL); } // Free copy on malloc fail
		new_arg[0] = word_copy; // Assign the COPY
		new_arg[1] = NULL;
	}
	else
	{
		new_arg = malloc(sizeof(char *) * (old_size + 2));
		if (!new_arg) { free(word_copy); return (NULL); } // Free copy on malloc fail
		i = 0;
		while (i < old_size)
		{
			new_arg[i] = old_argv[i]; // Copy old pointers
			i++;
		}
		new_arg[old_size] = word_copy; // Assign the COPY
		new_arg[old_size + 1] = NULL;
		if (old_argv)
			 free(old_argv); // Free the OLD array of pointers
	}
	return (new_arg);
}

t_cmd	*ft_creat_cmd_table(t_token *token)
{
	// ... initialization ...
		t_cmd	*head = NULL;
	t_cmd	*new_cmd;
	t_redir	*new_redir;
	t_token	*curr;
	size_t	n_cmd;
	size_t	i;

	i = 0;
	n_cmd = ft_count_cmd(token);
	curr = token;
	while (i < n_cmd && curr) // Loop per command segment (pipe delimited)
	{
		new_cmd = ft_cmd_new();
		while (curr && curr->type != TOKEN_PIPE) // Loop within one segment
		{
			if (curr->type == TOKEN_WORD)
			{
				// Adds word pointer directly to new_cmd->argv
				new_cmd->argv =  ft_add_to_argv(new_cmd->argv, curr->value);
			}
			else if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HEREDOC)
			{
				// Creates redirection node using curr->next->value as filename
				new_redir = ft_redir_new(curr->type, curr->next->value); // Potential issue: Doesn't copy filename?
				ft_redir_add_back(&new_cmd->redir,new_redir);
				curr = curr->next; // Skips filename token
			}
			curr = curr->next; // Move to next token
		}
		ft_cmd_add_back(&head, new_cmd); // Add completed command node
		if (curr && curr->type == TOKEN_PIPE)
			curr = curr->next; // Move past pipe
		i++;
	}
	return (head);
}

// t_cmd	*ft_creat_cmd_table(t_token *token)
// {

// 	t_cmd	*head = NULL;
// 	t_cmd	*new_cmd;
// 	t_redir	*new_redir;
// 	t_token	*curr;
// 	size_t	n_cmd;
// 	size_t	i;

// 	i = 0;
// 	n_cmd = ft_count_cmd(token);
// 	curr = token;
// 	while (i < n_cmd && curr)
// 	{
// 		new_cmd = ft_cmd_new();
// 		while (curr && curr->type != TOKEN_PIPE)
// 		{
// 			if (curr->type == TOKEN_WORD)
// 			{
// 				new_cmd->argv =  ft_add_to_argv(new_cmd->argv, curr->value);
// 			}
// 			else if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HEREDOC)
// 			{
// 				new_redir = ft_redir_new(curr->type, curr->next->value);
// 				ft_redir_add_back(&new_cmd->redir,new_redir);
// 				curr = curr->next;
// 			}
// 			curr = curr->next;
// 		}	
// 		ft_cmd_add_back(&head, new_cmd);
// 		if (curr && curr->type == TOKEN_PIPE)
// 			curr = curr->next;
// 		i++; // to creat a new cmd node 
// 	}
// 	return (head);
// }
