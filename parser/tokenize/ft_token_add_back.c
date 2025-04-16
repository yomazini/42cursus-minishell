#include "../parser.h" 

static t_token *ft_token_last(t_token *token)
{
    if (!token)
        return (NULL);
    while (token->next) 
    {
        token = token->next;
    }
    return (token);
}
void ft_token_add_back(t_token **token_list, t_token *new_node)
{
    t_token *last;

    if (!token_list || !new_node)
        return;

    if (*token_list)
    {
        last = ft_token_last(*token_list);
        last->next = new_node;
    }
    else
        *token_list = new_node;
}