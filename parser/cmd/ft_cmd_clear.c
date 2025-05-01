#include "../parser.h"



// static void	ft_cmd_delone(t_cmd *node)
// {

// 	if (!node)
// 		return ;
// 	if (node->argv)
// 	{
// 		free(node->argv);
// 		node->argv = NULL;
// 	}
// 	if (node->redir)
// 		ft_redir_clear(&node->redir);
// 	free(node);
// }
void	free_arr(char **arr);

static void	ft_cmd_delone(t_cmd *node)
{
	if (!node)
		return ;

	// *** FIX: Use free_arr to free strings inside argv first ***
	if (node->argv)
	{
		free_arr(node->argv); // Frees strings AND the array itself
		node->argv = NULL;    // Set to NULL after freeing
	}
	// *** END FIX ***

	if (node->redir) // Assuming 'redir' is the correct name from parser.h
		ft_redir_clear(&node->redir); // Ensure ft_redir_clear handles its freeing correctly

	free(node); // Free the t_cmd struct itself
}


void	ft_cmd_clear(t_cmd **cmd_list)
{
	t_cmd	*current;
	t_cmd	*next_node;

	if (!cmd_list || !*cmd_list)
		return ;
	current = *cmd_list;
	while (current)
	{
		next_node = current->next;	
		ft_cmd_delone(current);
		current = next_node;
	}
	*cmd_list = NULL;
	 
}