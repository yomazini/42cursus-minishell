/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:46:52 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/08 00:28:22 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

// THE ONLY ALLOWED GLOBAL VARIABLE
// volatile sig_atomic_t	g_signal_received = 0;

// // Handler for SIGINT (Ctrl+C) when waiting for prompt input
// void	handle_sigint_prompt(int signum)
// {
// 	(void)signum;
// 	g_signal_received = SIGINT; // Set the flag
// 	write(STDOUT_FILENO, "\n", 1); // Move to a new line
// 	rl_on_new_line(); // Tell readline cursor moved
// 	// rl_replace_line("", 0); // Clear current buffer (optional, redisplay handles)
// 	rl_redisplay();     // Redisplay the prompt and cleared buffer
// }

// // Handler for SIGINT (Ctrl+C) when reading heredoc input
// void	handle_sigint_heredoc(int signum)
// {
// 	(void)signum;
// 	g_signal_received = SIGINT; // Set the global flag
// 	// The heredoc reading loop needs to see this flag.
// 	// Closing stdin used to be a way to force readline to return,
// 	// but setting rl_catch_signals=0 should make readline return EINTR.
// 	// We just need to ensure the flag is checked *after* readline returns.
// 	// Print newline to mimic bash ^C during heredoc
// 	write(STDERR_FILENO, "\n", 1);
// 	// No redisplay here, main loop handles prompt after heredoc abortion
//     close(STDIN_FILENO); // Force readline to return in read_input_to_pipe
// }

// // Function to set signal actions
// void	set_signal_action(int signum, void (*handler)(int), int flags)
// {
// 	struct sigaction	sa;

// 	sa.sa_handler = handler;
// 	sigemptyset(&sa.sa_mask); // Don't block other signals during handler
// 	sa.sa_flags = flags;      // e.g., SA_RESTART or 0
// 	sigaction(signum, &sa, NULL); // Apply the action
// }

/*	--->> V 2.0 	*/

// volatile sig_atomic_t	g_signal_received = 0;

// // For Prompt (main loop)
// void	handle_sigint_prompt(int signum)
// {
// 	(void)signum;
// 	g_signal_received = SIGINT;
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line(); // Needed BEFORE replace/redisplay
// 	rl_replace_line("", 0); // Clear buffer
// 	rl_redisplay();
// }

// // For Heredoc Reading
// void	handle_sigint_heredoc(int signum)
// {
// 	(void)signum;
// 	// NOTE: This handler might run *while* readline is blocked.
// 	// Setting the flag is the primary goal. Closing stdin might be
// 	// necessary if readline doesn't return EINTR automatically on signal.
// 	g_signal_received = SIGINT;
// 	close(STDIN_FILENO); // Force readline to unblock and return NULL/error
// 	// Write newline to stderr to avoid mixing with potential command output later
// 	write(STDERR_FILENO, "\n", 1);
// 	// No readline redisplay needed here, main loop handles next prompt
// }

// // Utility to set signal actions
// void	set_signal_action(int signum, void (*handler)(int), int flags)
// {
// 	struct sigaction	sa;

// 	ft_memset(&sa, 0, sizeof(sa)); // Initialize struct
// 	sa.sa_handler = handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = flags;
// 	if (sigaction(signum, &sa, NULL) == -1)
// 	{
// 		perror("minishell: sigaction failed");
// 		// Handle error? Maybe exit if initial setup fails?
// 	}
// }