#include "parser.h"

int main(void)
{
	char	*line;
    t_token *token_list_head = NULL;
    //~ one-time setup; ensures the pointer is not holding a garbage value

	while ((line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002")))
	{
        token_list_head = NULL;
        // //~ init = NULL per-command reset; after each tokenize list
		if (line && *line && ft_synax_error_free(line))
        {
			add_history(line);
            token_list_head = ft_tokenize(line);
            if (token_list_head)
                ft_print_token_list(token_list_head);
        }
		free(line);
        ft_token_clear(&token_list_head);
        token_list_head = NULL;
		line = NULL;
	}
    rl_clear_history();
	return (0);	
}