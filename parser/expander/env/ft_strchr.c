#include "../../parser.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*ptr;

	ptr = (char *)s;
	if ((char)c == '\0')
		return (ptr + ft_strlen(s));
	i = 0;
	while (ptr[i])
	{
		if (ptr[i] == (char)c)
			return (&ptr[i]);
		i++;
	}
	return (NULL);
}