#include "../../parser.h"

void	ft_tenv_delone(t_env *node)
{
	if (!node)
		return ;
	if (node->name)
	{
		free(node->name);
		node->name = NULL;
	}
	if (node->value)
	{
		free(node->value);
		node->value = NULL;
	}
	free(node);
}
