#include "../parser.h"

static char	ft_get_alpha_quote(char *word)
{
	int		i;
	char	quote;
	int		at_quote;

	i = 0;
	at_quote = 0;
	quote = '\0';
	while(word[i])
	{
		if (ft_isquot(word[i]))
		{
			quote = word[i];
			at_quote = 1;
		}
		else if (ft_isquot(word[i]) && at_quote == 1)
			break;
		i++;
	}
	return (quote);
}

void	ft_clean_up(t_token **token)
{
	char	alpha_quote;
	char	*new_string;
	int		i = 0;
	int		j = 0;
	size_t	len;

	if (!token || !*token || !(*token)->value)
		return;
	alpha_quote = ft_get_alpha_quote((*token)->value);
	if (!alpha_quote)
		return ;
	len = ft_strlen((*token)->value);
	new_string = malloc(sizeof(char) * (len + 1));
	if (!new_string)
		return ;
	while ((*token)->value[i])
	{
		if ((*token)->value[i] != alpha_quote)
			new_string[j++] = (*token)->value[i];
		i++;
	}
	new_string[j] = '\0';
	free((*token)->value);
	(*token)->value = new_string;
}