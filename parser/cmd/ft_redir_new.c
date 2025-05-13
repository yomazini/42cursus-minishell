/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_new.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:09:03 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/13 16:30:15 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

t_redir	*ft_redir_new(int type, char *filename_val, bool herdoc)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->type = type;
	new_redir->filename = ft_strdup(filename_val);
	if (!new_redir->filename)
	{
		free(new_redir);
		return (NULL);
	}
	new_redir->heredoc_fd = -1;
	if (new_redir->type == TOKEN_REDIR_HEREDOC && herdoc == true)
		new_redir->expand_heredoc = true;
	else
		new_redir->expand_heredoc = false;
	new_redir->next = NULL;
	return (new_redir);
}
