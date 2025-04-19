#include "../../parser.h"

char	*ft_strdup(const char *string)
{
	size_t	size;
	char	*cpy;
	int		i;

	if (!string)
		return (NULL);
	size = ft_strlen((char *)string);
	cpy = malloc(sizeof(char) * (size + 1));
	if (!cpy)
		return (NULL);
	i = 0;
	while (string[i])
	{
		cpy[i] = string[i];
		i++;
	}
	cpy[size] = '\0';
	return (cpy);
}