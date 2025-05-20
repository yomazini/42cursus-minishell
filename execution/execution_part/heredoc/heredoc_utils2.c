/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:12:00 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/20 13:42:34 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../exec_header.h" 

int	count_total_heredocs(t_cmd *cmd_list)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;
	int		count;

	current_cmd = cmd_list;
	count = 0;
	while (current_cmd)
	{
		current_redir = current_cmd->redir;
		while (current_redir)
		{
			if (current_redir->type == TOKEN_REDIR_HEREDOC)
				count++;
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (count);
}

void	cleanup_all_heredoc_fds(t_cmd *cmd_list)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		current_redir = current_cmd->redir;
		while (current_redir)
		{
			if (current_redir->type == TOKEN_REDIR_HEREDOC
				&& current_redir->heredoc_fd != -1)
			{
				close(current_redir->heredoc_fd);
				current_redir->heredoc_fd = -1;
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}
