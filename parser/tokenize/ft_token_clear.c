#include "../parser.h"

void    ft_token_clear(t_token **token_list)
{
    t_token *current;
    t_token *next_node;

    if (!token_list || !*token_list)
        return ;
    current = *token_list;
    while(current)
    {
        next_node = current->next;
        ft_token_delone(current);
        current = next_node;
    }
    *token_list = NULL;
}