#include "../../parser.h"

void	ft_tenv_add_back(t_env **env_list, t_env *new_node)
{
	t_env	*last;

	if (!env_list || !new_node)
		return ;
	if (*env_list)
	{
		last = ft_tenv_last(*env_list);
		last->next = new_node;
	}
	else
		*env_list = new_node;
}
