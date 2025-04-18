#include "../../parser.h"

t_env	*ft_tenv_last(t_env *env)
{
	if (!env)
		return (NULL);
	while (env->next)
	{
		env = env->next;
	}
	return (env);
}
