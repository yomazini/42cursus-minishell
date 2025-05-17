/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:00:25 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/17 16:17:45 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

void set_signal_handlers_ignore(void)
{
    configure_sigaction(SIGINT, SIG_IGN, 0);
    configure_sigaction(SIGQUIT, SIG_IGN, 0);
}

void restore_signal_handlers(struct sigaction *old_sigint,
                             struct sigaction *old_sigquit)
{
    if (old_sigint)
        sigaction(SIGINT, old_sigint, NULL);
    if (old_sigquit)
        sigaction(SIGQUIT, old_sigquit, NULL);
}


void	set_signal_handlers_prompt(void)
{
	configure_sigaction(SIGINT, sigint_handler_prompt, SA_RESTART);
	configure_sigaction(SIGQUIT, SIG_IGN, 0);
}


void	set_signal_handlers_default(void)
{
	configure_sigaction(SIGINT, SIG_DFL, 0);
	configure_sigaction(SIGQUIT, SIG_DFL, 0);
}