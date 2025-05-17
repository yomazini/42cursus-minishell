/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_herdoc_signal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:17:27 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/17 16:17:50 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	sigint_handler_heredoc(int signum)
{
	extern int	g_tmp;
	(void)signum;
	g_tmp = 3;
	close(STDIN_FILENO);
}

void	set_signal_handlers_heredoc(void)
{
	configure_sigaction(SIGINT, sigint_handler_heredoc, 0);
	configure_sigaction(SIGQUIT, SIG_IGN, 0);
}