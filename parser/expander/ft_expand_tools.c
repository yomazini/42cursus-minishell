#include "../parser.h"

int ft_isexpandable(t_token *current)
{
	if (current->type == TOKEN_WORD && (ft_strchr(current->value, '$')))
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
	else if (c >= '0' && c <= '9')
		return (2);
	else if (c == '?')
		return (3);
	else if (c == '$')
		return (4);
	// else if (ft_isspecial_char(c))
	// 	return (5);
	return (-1);
}
