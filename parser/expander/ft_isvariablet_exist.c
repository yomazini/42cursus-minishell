#include "../parser.h"

char	*ft_isvariablet_exist(t_env *env_list, char *variable_name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (current->name && ft_strcmp(current->name, variable_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}