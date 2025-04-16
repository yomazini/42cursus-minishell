#include "../parser.h"

size_t ft_strlen(const char *string)
{
    size_t  string_len;

    string_len = 0;
    while (string[string_len])
        string_len++;
    return (string_len); 
}