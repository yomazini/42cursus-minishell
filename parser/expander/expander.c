#include "../parser.h"

void	ft_expander(t_token **token, t_env *env)
{
	t_token *current;

	current = *token;
	(void)env;
	while (current)
	{
		if (ft_isexpandable(current))
		{
			ft_expand(&current, env);
		}
		ft_clean_up(&current);
		current = current->next;
	}
}
