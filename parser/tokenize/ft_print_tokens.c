#include "../parser.h"

const char* get_token_type_name(t_token_type type) {
    switch (type) {
        case TOKEN_WORD:         return "WORD";
        case TOKEN_PIPE:         return "PIPE";
        case TOKEN_REDIR_IN:     return "REDIR_IN";
        case TOKEN_REDIR_OUT:    return "REDIR_OUT";
        case TOKEN_REDIR_APPEND: return "REDIR_APPEND"; // Make sure you have this enum
        case TOKEN_REDIR_HEREDOC:return "REDIR_HEREDOC";
        // Add cases for any other token types you might have
        default:                 return "UNKNOWN";
    }
}
void ft_print_token_list(t_token *head)
{
    t_token *current = head; 
    int i = 0;   
    while (current != NULL)
    {
        // Print in the requested format
        printf("token[%d]= [%s], type(%s)\n",
               i,
               current->value ? current->value : "(null value)", // Safety check
               get_token_type_name(current->type)); // Get type name string

        // Move to the next node in the list
        current = current->next;
        i++; // Increment index
    }
}