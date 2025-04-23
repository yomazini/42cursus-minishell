#include "../parser.h"

static char	*ft_filljoin(char *tofill, const char *str_buf, const char *buf)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str_buf[i])
	{
		tofill[i] = str_buf[i];
		i++;
	}
	j = 0;
	while (buf[j])
	{
		tofill[i] = buf[j];
		i++;
		j++;
	}
	tofill[i] = '\0';
	return (tofill);
}

char	*ft_strjoined(char const *string, char const *str)
{
	char	*strjn;
	size_t	len;

	if (!string && !str)
		return (NULL);
	if (!string)
		return (ft_strdup(str));
	if (!str)
		return (ft_strdup(string));
	len = ft_strlen(string) + ft_strlen(str);
	strjn = malloc((len + 1) * sizeof(char));
	if (!strjn)
		return (NULL);
	strjn = ft_filljoin(strjn, string, str);
	return (strjn);
}