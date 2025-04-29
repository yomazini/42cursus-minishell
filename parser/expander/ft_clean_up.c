#include "../parser.h"

void	ft_clean_up(t_token **token)
{
	char	alpha_quote;
	char	*new_str;
	int		i = 0;
	int		j = 0;
	size_t	len;


	if (!token || !*token || !(*token)->value)
		return;
	alpha_quote = '\0';
	len = ft_strlen((*token)->value);
	new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return ;
	while ((*token)->value[i])
	{
		if(!alpha_quote &&  ft_isquot((*token)->value[i]))
		{
			alpha_quote = (*token)->value[i];
			i++;
		}
		else if (alpha_quote && (*token)->value[i] == alpha_quote)
		{
			i++;
			alpha_quote = '\0';
		}
		else
			new_str[j++] = (*token)->value[i++];
	}
	new_str[j] = '\0';
	free((*token)->value);
	(*token)->value = new_str;
}