#include "../parser.h"

int ft_isdouble_quote(char *word)
{
	int		i;
	char	quote;
	int		at_quote;

	i = 0;
	at_quote = 0;
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
	if (quote == '\"' || at_quote == 0)
		return (1);
	return (0);
}
int ft_isexpandable(t_token *current)
{
	if (current->type == TOKEN_WORD && (ft_strchr(current->value, '$') 
		&& ft_isdouble_quote(current->value)))
		return (1);
	return(0);     
}

int	ft_isbign_variable(char c)
{
	return((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c  == '_'));
}

int ft_ispt_variable(char c)
{
	return((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9')
		|| c == '_');
}

int	ft_peakahead(char c)
{
	if (ft_isspace(c))
		return (0);
	else if (ft_isbign_variable(c))
	return (1);
	else if (c == '?')
		return (2);
	else if (c >= '0' && c <= '9')
		return (3);
	else if (ft_isquot(c))
		return (4);
	return (-1);
}
