#include "../parser.h"

int is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN ||
			type == TOKEN_REDIR_OUT ||
			type == TOKEN_REDIR_APPEND ||
			type == TOKEN_REDIR_HEREDOC);
}
t_token_type ft_token_type(char *token)
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

int ft_synatx_error(t_token *head)
{
	t_token *current;

	current = head;
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (is_redirection(current->next->type))
			{
				printf("\033[1;31mSyntax error:\033[0m \033[1;36mconsecutive redirection operators\033[0m\n");
				return (1);
			}
			else if (current->next->type == TOKEN_PIPE)
			{
				printf("\033[1;31mSyntax error:\033[0m \033[1;36mnear unexpected token '|'\033[0m\n");
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}


t_token *ft_tokenize(char *line)
{
    size_t i = 0;
    char **tkn_array = NULL;
    t_token *head = NULL;
    t_token *new_node = NULL;
    t_token_type type;

    if (!line)
        return (NULL);
    tkn_array = ft_split_tokens(line);
    if (!tkn_array)
        return (NULL);
    while (tkn_array[i])
    {
        type = ft_token_type(tkn_array[i]);
        new_node = ft_token_new(tkn_array[i], type);
        if (!new_node)
            return (free(tkn_array), ft_token_clear(&head), NULL);
        ft_token_add_back(&head, new_node);
        i++;
    }
    free(tkn_array);
    tkn_array = NULL;
	if (ft_synatx_error(head))
		return (ft_token_clear(&head),NULL);
    return (head); 
}
