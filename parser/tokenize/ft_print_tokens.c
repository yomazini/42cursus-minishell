#include "../parser.h"

const char	*get_token_type_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	else if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == TOKEN_REDIR_APPEND)
		return ("REDIR_APPEND");
	else if (type == TOKEN_REDIR_HEREDOC)
		return ("REDIR_HEREDOC");
	return ("UNKNOWN");
}

void	ft_print_token_list(t_token *head)
{
	t_token	*current;
	int		i;

	current = head;
	i = 0;
	while (current != NULL)
	{
		printf("token[%d]= [%s], type(%s)\n",
			i,
			current->value ? current->value : "(null value)",
			get_token_type_name(current->type));
		current = current->next;
		i++;
	}
}
