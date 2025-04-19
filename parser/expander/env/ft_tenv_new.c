#include "../../parser.h"

t_env	*ft_tenv_new(char *name, char *value, int flag)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->name = name;
	new_node->value = value;
	new_node->flag_env = flag;
	new_node->next = NULL;
	return (new_node);
}