#include "../parser.h"

void    ft_token_delone(t_token *node)
{
    if (!node)
        return ;
    if (node->value)
    {
        free(node->value);
        node->value = NULL;
    }
    free(node);
}