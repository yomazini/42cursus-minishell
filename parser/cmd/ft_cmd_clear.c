#include "../parser.h"

static void	ft_cmd_delone(t_cmd *node)
{

	if (!node)
		return ;
	if (node->argv)
	{
		free(node->argv);
		node->argv = NULL;
	}
	if (node->redir)
		ft_redir_clear(&node->redir);
	free(node);
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