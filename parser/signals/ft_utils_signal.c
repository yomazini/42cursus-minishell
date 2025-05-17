/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_signal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:14:23 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/17 16:17:23 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void	setup_signal_action(int signum, void (*handler)(int), int flags)
{
    struct sigaction	sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = flags;
    sigaction(signum, &sa, NULL);
}

void	sigint_handler_prompt(int signum)
{
	extern int g_tmp;

	(void)signum;
	if (g_tmp == 1)
	{
		g_tmp = 0;
		// write(STDOUT_FILENO, "\n", 1);
	}
	else
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_tmp = 2;
	}	
}


void	configure_sigaction(int signum, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(signum, &sa, NULL) == -1)
	{
		perror("minishell: sigaction setup failed");
	}
}

void set_parent_wait_signal_handlers(struct sigaction *old_sigint,
                                     struct sigaction *old_sigquit)
{
    struct sigaction sa_ign;

    sa_ign.sa_handler = SIG_IGN;
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = 0;
    sigaction(SIGINT, &sa_ign, old_sigint);
    sigaction(SIGQUIT, &sa_ign, old_sigquit);
}