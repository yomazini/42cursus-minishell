#include "../parser.h"

void	ft_spacial_case(t_token **token_ptr)
{
	char	*exp_value;

	if(!token_ptr|| !*token_ptr || !(*token_ptr)->value)
		return ;
	if (ft_strnstr((*token_ptr)->value, "$'", ft_strlen((*token_ptr)->value)))
	{
		if ((*token_ptr)->value[0] == '$')
		{
			exp_value = ft_strdup(&((*token_ptr)->value[1]));
			if (!exp_value)
				return;
			free((*token_ptr)->value);
			(*token_ptr)->value = exp_value;
		}
	}
}
void	ft_expander(t_token **token, t_env *env)
{
	t_token *current;

	current = *token;
	while (current)
	{
		if (ft_isexpandable(current))
			ft_expand(&current, env);
		else
			ft_spacial_case(&current);
		ft_clean_up(&current);
		current = current->next;
	}
}
