#include "../parser.h"

// t_redir	*ft_redir_new(int type, char *filename)
// {
// 	t_redir	*new_redir;

// 	new_redir = malloc(sizeof(t_redir));
// 	if (!new_redir)
// 		return (NULL);
// 	new_redir->type = type;
// 	new_redir->filename = filename;
// 	new_redir->heredoc_fd = -1;
// 	if (new_redir->type == TOKEN_REDIR_HEREDOC)
// 		new_redir->expand_heredoc = true;
// 	else
// 		new_redir->expand_heredoc = false;
// 	new_redir->next = NULL;
// 	return (new_redir);
// }

t_redir	*ft_redir_new(int type, char *filename_val) // Changed param name
{
    t_redir *new_redir;

    new_redir = malloc(sizeof(t_redir));
    if (!new_redir) return (NULL);
    new_redir->type = type;
    // *** FIX: Duplicate the filename value ***
    new_redir->filename = ft_strdup(filename_val);
    if (!new_redir->filename) { // Check strdup result
        free(new_redir);
        return (NULL);
    }
    // *** END FIX ***
    new_redir->heredoc_fd = -1;
    if (new_redir->type == TOKEN_REDIR_HEREDOC)
		new_redir->expand_heredoc = true;
	else
		new_redir->expand_heredoc = false;
	new_redir->next = NULL;
    return (new_redir);
}

