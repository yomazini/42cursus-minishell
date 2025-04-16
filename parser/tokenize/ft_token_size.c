#include "../parser.h"

int ft_token_size(t_token *head)
{
    int size;
    t_token *current;

    current = head;
    size = 0;
    while (current)
    {
        size++;
        current = current->next;
    }
    return (size);   
}