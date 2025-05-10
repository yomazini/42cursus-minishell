#include "includes/minishell.h" 
#include "parser/parser.h"
#include "execution/exec_header.h"
#include <signal.h>

// int g_child_open = 0;

const char* get_token_type_name(t_token_type type) {
    switch (type) {
        case TOKEN_WORD:         return "WORD";
        case TOKEN_PIPE:         return "PIPE";
        case TOKEN_REDIR_IN:     return "REDIR_IN";
        case TOKEN_REDIR_OUT:    return "REDIR_OUT";
        case TOKEN_REDIR_APPEND: return "REDIR_APPEND";
        case TOKEN_REDIR_HEREDOC:return "REDIR_HEREDOC";
        default:                 return "UNKNOWN";
    }
}
void ft_print_token_list(t_token *head)
{
    t_token *current = head; 
    int i = 0;   
    while (current != NULL)
    {
        printf("token[%d]= [%s], type(%s)\n",
               i,
               current->value ? current->value : "(null value)",
               get_token_type_name(current->type));
        current = current->next;
        i++;
    }
}



void ft_print(char **argv)
{
    int i = 0;

    // *** ADD THIS CHECK ***
    if (argv == NULL)
    {
        printf("[ (no arguments) ]\n"); // Or just print nothing
        return;
    }
    // *** END CHECK ***

    // Original loop is fine if argv is not NULL
    while (argv[i] != NULL)
    {
        printf("[%s] ", argv[i]); // Maybe remove trailing comma/newline here
        i++;
    }
     printf("\n"); // Add newline after printing all args for one command
}

// Optional: Adjust ft_print_cmd_table slightly for formatting
void    ft_print_cmd_table(t_cmd *head)
{
    t_cmd *curr_cmd = head;
    t_redir *curr_redir;
    int cmd_num = 0;

    while (curr_cmd)
    {
        printf("--- Command %d ---\n", cmd_num);
        printf("  Args: "); // Label for arguments
        ft_print(curr_cmd->argv); // ft_print now handles NULL and adds newline

        if (curr_cmd->redir)
        {
             printf("  Redirs:\n"); // Label for redirections
            curr_redir = curr_cmd->redir;
            while (curr_redir)
            {
                // Use the helper function from before for readable types
                // const char *type_str = ft_redir_type_to_str(curr_redir->type);
                // printf("    type: %s ", type_str);
                printf("    type: %d ", curr_redir->type); // Original version
                printf("bool: %d ", curr_redir->expand_heredoc);
                printf("filename: %s " , curr_redir->filename ? curr_redir->filename : "(null)");
                printf("fd: %d\n", curr_redir->heredoc_fd);
                curr_redir = curr_redir->next;
            }
        } else {
             printf("  Redirs: (None)\n");
        }
        printf("------------------\n");
        curr_cmd = curr_cmd->next;
        cmd_num++;
    }
}


// --- Define the Global Signal Variable ---
// volatile sig_atomic_t	g_received_signal = 0;

// // --- Signal Handlers ---
// void	main_prompt_sigint_handler(int signum)
// {
// 	(void)signum;
// 	g_received_signal = SIGINT;
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// void	heredoc_sigint_handler(int signum)
// {
// 	(void)signum;
// 	g_received_signal = SIGINT; // Set flag
// 	// To unblock readline, we close a different FD, NOT STDIN directly
// 	// This requires a global pipe or a way to pass FD to handler.
// 	// Simpler: rely on readline returning NULL or error on EINTR.
// 	// For now, just setting flag and printing newline for feedback.
// 	write(STDERR_FILENO, "\n", 1); // Bash prints newline after ^C in heredoc
// 	// Readline should return NULL if rl_catch_signals = 0 and handler runs.
// 	// We check g_received_signal in the heredoc loop.
//     close(STDIN_FILENO); // Force readline to return (can be risky if stdin was not terminal)
//                          // This is a common way to make readline return on signal
//                          // if SA_RESTART is not used or readline doesn't handle EINTR well.
// }

// // --- Setup Functions for Signal Actions ---
// void	setup_signal_action(int signum, void (*handler)(int), int flags)
// {
// 	struct sigaction	sa;

// 	sa.sa_handler = handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = flags; // Typically SA_RESTART for prompt, 0 for heredoc/child
// 	if (sigaction(signum, &sa, NULL) == -1)
// 		perror("minishell: sigaction setup failed");
// }

// // Setup for main prompt
// void	setup_signal_handlers_prompt(void)
// {
// 	setup_signal_action(SIGINT, main_prompt_sigint_handler, SA_RESTART);
// 	setup_signal_action(SIGQUIT, SIG_IGN, 0); // Ignore Ctrl+\ at prompt
// }

// // Setup for child processes (default behavior)
// void	set_child_signal_handlers(void)
// {
// 	setup_signal_action(SIGINT, SIG_DFL, 0);
// 	setup_signal_action(SIGQUIT, SIG_DFL, 0);
// }

// // Setup for parent when waiting for children
// void	set_parent_wait_signal_handlers(struct sigaction *old_sigint,
// 										struct sigaction *old_sigquit)
// {
// 	struct sigaction	sa_ign;

// 	sa_ign.sa_handler = SIG_IGN; // Ignore SIGINT and SIGQUIT
// 	sigemptyset(&sa_ign.sa_mask);
// 	sa_ign.sa_flags = 0;
// 	sigaction(SIGINT, &sa_ign, old_sigint);   // Save old SIGINT (prompt handler)
// 	sigaction(SIGQUIT, &sa_ign, old_sigquit); // Save old SIGQUIT (SIG_IGN)
// }

// // Restore original/prompt handlers (used by parent after wait or by heredoc)
// void	restore_signal_handlers(struct sigaction *old_sigint,
// 								struct sigaction *old_sigquit)
// {
// 	if (old_sigint)
// 		sigaction(SIGINT, old_sigint, NULL);
// 	if (old_sigquit)
// 		sigaction(SIGQUIT, old_sigquit, NULL);
// }


// int	main(int ac, char **av, char **env)
// {
// 	t_data				data;
// 	char				*line;
// 	t_token				*token_list;
// 	t_cmd				*command_list;
// 	struct sigaction	old_parent_sigint; // To store handlers when waiting
// 	struct sigaction	old_parent_sigquit;

// 	(void)ac; (void)av;
// 	line = NULL; token_list = NULL; command_list = NULL;
// 	data.last_exit_status = EXIT_SUCCESS;
// 	data.env_list = ft_getenv(env);
// 	if (!data.env_list && env && env[0])
// 		ft_putstr_fd("minishell: Warning: env list init failed.\n", 2);
// 	update_shell_level(&data);
// 	rl_catch_signals = 0; // IMPORTANT: Let our handlers work
// 	setup_signal_handlers_prompt(); // Initial prompt signal setup

// 	while (TRUE)
// 	{
// 		command_list = NULL; token_list = NULL; line = NULL;
// 		if (g_received_signal == SIGINT) // Handle SIGINT that occurred at prompt
// 		{
// 			data.last_exit_status = 130;
// 			g_received_signal = 0; // Reset flag
// 		}
// 		line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
// 		if (!line) // EOF (Ctrl+D or readline interrupted by signal)
// 		{
// 			// If g_signal_received is SIGINT here, it means heredoc_sigint_handler
// 			// closed stdin, causing readline to return NULL.
// 			if (g_received_signal == SIGINT) {
// 				// main_prompt_sigint_handler would have been restored by process_heredocs
// 				// and it will do the rl_on_new_line etc.
// 				// The flag g_received_signal will be handled at the top of the next loop.
// 				// No "exit" message here if it was a heredoc ^C.
// 				continue;
// 			}
// 			ft_putstr_fd("exit\n", STDOUT_FILENO); // Actual Ctrl+D at prompt
// 			break ;
// 		}
// 		if (line[0] == '\0') { free(line); continue; }
// 		add_history(line);
// 		if (!ft_synax_error_free(line)) { data.last_exit_status = 258; free(line); continue; }
// 		token_list = ft_tokenize(line);
// 		if (!token_list) { data.last_exit_status = 258; free(line); continue; }

// 		ft_expander(&token_list, &data); // Pass &data
// 		command_list = ft_creat_cmd_table(token_list);
// 		ft_token_clear(&token_list);

// 		if (command_list)
// 		{
// 			g_received_signal = 0; // Reset flag before potentially blocking operations
// 			if (process_heredocs(command_list, &data) == EXIT_SUCCESS && g_received_signal != SIGINT)
// 			{
// 				set_parent_wait_signal_handlers(&old_parent_sigint, &old_parent_sigquit);
// 				execute_commands(command_list, &data);
// 				restore_signal_handlers(&old_parent_sigint, &old_parent_sigquit);
// 			}
// 			else if (g_received_signal == SIGINT) // Heredoc was interrupted
// 			{
// 				data.last_exit_status = 130;
// 				// g_received_signal is reset at top of loop
// 				// ensure prompt handlers are restored if process_heredocs didn't
// 				restore_signal_handlers(&old_parent_sigint, &old_parent_sigquit);
// 			}
// 			// else: process_heredocs failed for non-signal reason, status already set
// 			ft_cmd_clear(&command_list);
// 		}
// 		else
// 		{
// 			data.last_exit_status = 2; // should it be 2 or 258 ?? Why ? and How ?
// 			printf("[Command Table Creation Failed - Check Syntax]\n");
// 			// here free command_list linked list
// 		}
// 		// ... (handle command_list NULL from ft_create_cmd_table) ...
// 		free(line);
// 	}
// 	ft_tenv_clear(&data.env_list);
// 	rl_clear_history();
// 	return (data.last_exit_status);
// }


// Function to set signal actions
void	setup_signal_action(int signum, void (*handler)(int), int flags)
{
    struct sigaction	sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = flags;
    sigaction(signum, &sa, NULL);
}

void	heredoc_sigint_handler(int signum)
{
    (void)signum;
    g_received_signal = SIGINT;
    // DO NOT CLOSE STDIN_FILENO HERE.
    // The readline call in read_input_to_pipe should return NULL
    // or its loop should check g_received_signal.
    // Printing newline here can be confusing if readline also prints one.
    // It's better to let process_heredocs handle the visual feedback of abortion.
    // write(STDERR_FILENO, "\n", 1); // Optional: if needed for immediate feedback
}


// --- Define the Global Signal Variable ---
volatile sig_atomic_t	g_received_signal = 0;

// --- Signal Handlers ---
void	sigint_handler_prompt(int signum)
{
	(void)signum;
	g_received_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler_heredoc(int signum)
{
	(void)signum;
	g_received_signal = SIGINT; // Set flag
	// The heredoc's readline loop needs to break.
	// Closing STDIN is a common way to force readline to return.
	// Make sure this STDIN is the one readline is using (usually terminal).
	close(STDIN_FILENO); // This will make readline in heredoc return NULL or error
	// Note: Bash prints newline after heredoc ^C, handled by process_heredocs
}

// --- Setup Functions for Signal Actions ---
static void	configure_sigaction(int signum, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(signum, &sa, NULL) == -1)
	{
		perror("minishell: sigaction setup failed");
		// Consider exiting if critical signal setup fails
	}
}
void set_signal_handlers_ignore(void) // Parent uses while waiting for child(ren)
{
    configure_sigaction(SIGINT, SIG_IGN, 0);
    configure_sigaction(SIGQUIT, SIG_IGN, 0);
}

// In minishell.c or signals.c
void set_parent_wait_signal_handlers(struct sigaction *old_sigint,
                                     struct sigaction *old_sigquit)
{
    struct sigaction sa_ign;

    sa_ign.sa_handler = SIG_IGN;
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = 0;
    // Save old handler and set new one to ignore
    sigaction(SIGINT, &sa_ign, old_sigint);
    sigaction(SIGQUIT, &sa_ign, old_sigquit);
}

void restore_signal_handlers(struct sigaction *old_sigint,
                             struct sigaction *old_sigquit)
{
    if (old_sigint)
        sigaction(SIGINT, old_sigint, NULL);
    if (old_sigquit)
        sigaction(SIGQUIT, old_sigquit, NULL);
}


// Setups for different shell states
void	set_signal_handlers_prompt(void)
{
	configure_sigaction(SIGINT, sigint_handler_prompt, SA_RESTART);
	configure_sigaction(SIGQUIT, SIG_IGN, 0);
}

void	set_signal_handlers_heredoc(void)
{
	// For heredoc, SIGINT should interrupt readline, so no SA_RESTART
	configure_sigaction(SIGINT, sigint_handler_heredoc, 0);
	configure_sigaction(SIGQUIT, SIG_IGN, 0); // Still ignore SIGQUIT
}

// void	set_signal_handlers_ignore(void) // Parent uses while waiting for child(ren)
// {
// 	configure_sigaction(SIGINT, SIG_IGN, 0);
// 	configure_sigaction(SIGQUIT, SIG_IGN, 0);
// }

void	set_signal_handlers_default(void) // Child process uses
{
	configure_sigaction(SIGINT, SIG_DFL, 0);
	configure_sigaction(SIGQUIT, SIG_DFL, 0);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*line;
	t_token	*token_list;
	t_cmd	*command_list;
	struct sigaction	old_parent_sigint;
	struct sigaction	old_parent_sigquit;

	(void)ac; (void)av;
	line = NULL; token_list = NULL; command_list = NULL;
	data.last_exit_status = EXIT_SUCCESS;
	data.env_list = ft_getenv(env);
	if (!data.env_list && env && env[0])
		ft_putstr_fd("minishell: Warning: env list init failed.\n", 2);
	update_shell_level(&data);
	rl_catch_signals = 0;         // Very important for our handlers to work
	set_signal_handlers_prompt(); // Initial setup for prompt

	while (TRUE)
	{
		command_list = NULL; token_list = NULL; line = NULL;
		// Check and reset global signal flag *before* readline
		if (g_received_signal == SIGINT) {
			data.last_exit_status = 130; // Status for SIGINT
			g_received_signal = 0;       // Reset flag
		}

		line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");

		if (!line) // EOF from readline
		{
			// If g_received_signal is SIGINT here, it means our heredoc_sigint_handler
			// closed STDIN, causing readline to return NULL.
			if (g_received_signal == SIGINT)
			{
				g_received_signal = 0; // Reset flag, status already 130 from above
				// Prompt handlers should have been restored by process_heredocs
				// No "exit" message here, main prompt will redraw.
				continue; // Go to next iteration to display fresh prompt
			}
			ft_putstr_fd("exit\n", STDOUT_FILENO); // Actual Ctrl+D at prompt
			break ;
		}

		if (line[0] == '\0') { free(line); continue; }
		add_history(line);

		if (!ft_synax_error_free(line)) { data.last_exit_status = 258; free(line); continue; }
		token_list = ft_tokenize(line);
		if (!token_list) { data.last_exit_status = 258; free(line); continue; }

		ft_expander(&token_list, &data);
		command_list = ft_creat_cmd_table(token_list);
		ft_token_clear(&token_list);
		if (command_list)
		{
			g_received_signal = 0;
			if (process_heredocs(command_list, &data) == EXIT_SUCCESS && g_received_signal != SIGINT)
			{
				// --- Parent ignores signals while children run ---
				set_parent_wait_signal_handlers(&old_parent_sigint, &old_parent_sigquit); // <-- CORRECTED CALL
				// --- End Parent Ignore ---

				execute_commands(command_list, &data);

				// --- Parent restores prompt signal handlers ---
				restore_signal_handlers(&old_parent_sigint, &old_parent_sigquit); // <-- Needs prototype
				// --- End Restore Handlers ---
			}
			else if (g_received_signal == SIGINT)
			{
				data.last_exit_status = 130;
				g_received_signal = 0;
				// Ensure prompt handlers are restored if process_heredocs was interrupted
				// and didn't get to its own restore.
				// If old_parent_sigint/quit were not set yet, this might be an issue.
				// Better: process_heredocs should ALWAYS restore before returning failure.
				// For now, if it was interrupted, main prompt handlers should be default.
				set_signal_handlers_prompt(); // Re-assert prompt handlers after ^C in heredoc
			}
			ft_cmd_clear(&command_list);

		}
		else
		{
			data.last_exit_status = 2; // should it be 2 or 258 ?? Why ? and How ?
			printf("[Command Table Creation Failed - Check Syntax]\n");
		}
		free(line);
	}
	ft_tenv_clear(&data.env_list);
	rl_clear_history();
	return (data.last_exit_status);
}