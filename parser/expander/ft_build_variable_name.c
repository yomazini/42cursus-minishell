#include "../parser.h"

static size_t	ft_vtsize(const char *string, int j)
{
	size_t	size = 0;

	while (string[j] && ft_ispt_variable(string[j]))
	{
		size++;
		j++;
	}
	return (size);
}

static char *ft_build_digit(const char *string, int *i)
{
	char *vt_name;

	vt_name = malloc(2);
	if (!vt_name)
		return (NULL);
	vt_name[0] = string[0];
	vt_name[1] = '\0';
	*i += 2;
	return (vt_name);
}

static char *ft_build_world(const char *string, int *i)
{
	size_t	vt_size;
	char	*vt_name;
	size_t	j;
	int		k;

	j = 0;
	k = 0; 
	vt_size = ft_vtsize(string, j);
	if (vt_size == 0)
		return (ft_strdup(""));
	vt_name = malloc(sizeof(char) * (vt_size + 1));
	if (!vt_name)
		return (NULL);
	while (j < vt_size)
			vt_name[k++] = string[j++];
	vt_name[k] = '\0';
	*i += vt_size + 1;
	return (vt_name);
}
char	*ft_build_variable_name(const char *string, int *i, int peak)
{
	char		*vt_name;

	if (peak == 3)
	{
		vt_name = ft_build_digit(string, i);
		if (!vt_name)
			return (NULL);
	}
	if (peak == 1)
	{	
		vt_name = ft_build_world(string, i);
		if (!vt_name)
			return (NULL);
	}
	return (vt_name);
}
