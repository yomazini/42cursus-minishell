#include "../../parser.h"

void	ft_tenv_clear(t_env **token_list)
{
	t_env	*current;
	t_env	*next_node;

	if (!token_list || !*token_list)
		return ;
	current = *token_list;
	while (current)
	{
		next_node = current->next;
		ft_tenv_delone(current);
		current = next_node;
	}
	*token_list = NULL;
}
