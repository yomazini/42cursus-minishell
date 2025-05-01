#include "../parser.h"

// static void	ft_redir_delone(t_redir *node)
// {
// 	if (!node)
// 		return ;
// 	free(node);
// }


static void	ft_redir_delone(t_redir *node)
{
    if (!node) return ;
    // *** FIX: Free the duplicated filename ***
    free(node->filename);
    node->filename = NULL;
    // *** END FIX ***
    // Close FD if needed: if (node->heredoc_fd >= 0) close(node->heredoc_fd);
    free(node);
}

void	ft_redir_clear(t_redir **redir_list)
{
	t_redir	*current;
	t_redir	*next_node;

	if (!redir_list || !*redir_list)
		return ;
	current = *redir_list;
	while (current)
	{
		next_node = current->next;	
		ft_redir_delone(current);
		current = next_node;
	}
	*redir_list = NULL;	 
}