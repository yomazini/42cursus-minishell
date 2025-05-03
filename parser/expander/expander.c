#include "../parser.h"

void	ft_expander(t_token **token, t_data *data)
{
	t_token *current;

	current = *token;
	while (current)
	{
		data->herdoc = false;
		if (current->type == TOKEN_REDIR_HEREDOC && current->next && current->next->type == TOKEN_WORD)
		{
			data->herdoc = true;
		}
		else if (ft_isexpandable(current))
		{
			ft_expand(&current, data);
		}
		ft_clean_up(&current);
		current = current->next;
	}
} 
