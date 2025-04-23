#include "../parser.h"

int ft_strcmp(const char *alpha, const char *bita) 
{
    size_t i;

	
	i = 0;
    while (alpha[i] != '\0' && bita[i] != '\0' && alpha[i] == bita[i])
        i++;
    if (alpha[i] == bita[i]) {
        return (0);
    } else {
        return (1);
    }
}