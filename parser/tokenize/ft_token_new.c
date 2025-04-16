#include "../parser.h"

t_token *ft_token_new(char *value, t_token_type type)
{
    t_token *new_node;

    new_node = malloc(sizeof(t_token));
    if (!new_node)
        return (NULL); 
    new_node->value = value; 
    new_node->type = type; 
    new_node->next = NULL;
    return (new_node);
}