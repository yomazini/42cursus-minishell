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
char	**ft_add_to_argv(char ** old_argv, char *word)
{
	char	**new_arg;
	size_t	old_size;
	size_t	i;

	// if (!word)
	// 	return (NULL);
	old_size = ft_viclen(old_argv);
	if (old_size == 0)
	{
		new_arg =  malloc(sizeof(char *) * 2);
		new_arg[0] = word;
		new_arg[1] = NULL;
	}
	else
	{
		new_arg = malloc(sizeof(char *) * (old_size + 2));
		if (!new_arg)
			return (NULL);
		i = 0;
		while (i < old_size)
		{
			new_arg[i] = old_argv[i];
			i++;
		}
		new_arg[old_size] = word;
		new_arg[old_size + 1] = NULL;
		if (old_argv)
			 free(old_argv);
	}
	return (new_arg);
}

t_cmd	*ft_creat_cmd_table(t_token *token)
{

	t_cmd	*head = NULL;
	t_cmd	*new_cmd;
	t_redir	*new_redir;
	t_token	*curr;
	size_t	n_cmd;
	size_t	i;

	i = 0;
	n_cmd = ft_count_cmd(token);
	curr = token;
	while (i < n_cmd && curr)
	{
		new_cmd = ft_cmd_new();
		while (curr && curr->type != TOKEN_PIPE)
		{
			if (curr->type == TOKEN_WORD)
			{
				new_cmd->argv =  ft_add_to_argv(new_cmd->argv, curr->value);
			}
			else if (curr->type >= TOKEN_REDIR_IN || curr->type <= TOKEN_REDIR_HEREDOC)
			{
				new_redir = ft_redir_new(curr->type, curr->next->value);
				ft_redir_add_back(&new_cmd->redir,new_redir);
				curr = curr->next;
			}
			curr = curr->next;
		}	
		ft_cmd_add_back(&head, new_cmd);
		if (curr && curr->type == TOKEN_PIPE)
			curr = curr->next;
		i++; // to creat a new cmd node 
	}
	return (head);
}
