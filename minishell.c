#include "includes/minishell.h" 
#include "parser/parser.h"
#include "execution/exec_header.h"
#include <signal.h>

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
//~__________________________________________________

volatile sig_atomic_t	g_received_signal = 0;
int g_tmp = 0;

void	setup_signal_action(int signum, void (*handler)(int), int flags)
{
    struct sigaction	sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = flags; //?
    sigaction(signum, &sa, NULL);
}

void	sigint_handler_prompt(int signum) // sigint handler for 
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

void	sigint_handler_heredoc(int signum) //! the same function two times 
{
	(void)signum;
	g_tmp = 3;
	close(STDIN_FILENO);
}

static void	configure_sigaction(int signum, void (*handler)(int), int flags) //! again two function do the same thing // flag ?
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

void set_signal_handlers_ignore(void)
{
    configure_sigaction(SIGINT, SIG_IGN, 0);
    configure_sigaction(SIGQUIT, SIG_IGN, 0);
}
void set_parent_wait_signal_handlers(struct sigaction *old_sigint,
                                     struct sigaction *old_sigquit) // set the 
{
    struct sigaction sa_ign;

    sa_ign.sa_handler = SIG_IGN; // not handler function just set to SIG_IGN
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = 0;
    sigaction(SIGINT, &sa_ign, old_sigint); //--> excute Handler for SIGINT and SIG_QUIT
    sigaction(SIGQUIT, &sa_ign, old_sigquit); // =
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

void	set_signal_handlers_heredoc(void)
{
	configure_sigaction(SIGINT, sigint_handler_heredoc, 0);
	configure_sigaction(SIGQUIT, SIG_IGN, 0);
}

void	set_signal_handlers_default(void)
{
	configure_sigaction(SIGINT, SIG_DFL, 0);
	configure_sigaction(SIGQUIT, SIG_DFL, 0);
}

// void	check_heap_leaks(void)
// {
// 	system("leaks minishell");  
// }


int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*line;
	t_token	*token_list;
	t_cmd	*command_list;
	struct sigaction	old_parent_sigint;
	struct sigaction	old_parent_sigquit;
	// atexit(check_heap_leaks);
	(void)ac; (void)av;
	line = NULL; token_list = NULL; command_list = NULL;
	data.last_exit_status = EXIT_SUCCESS;
	data.env_list = ft_getenv(env);
	if (!data.env_list && env && env[0])
		ft_putstr_fd("minishell: Warning: env list init failed.\n", 2);
	update_shell_level(&data);
	rl_catch_signals = 0;
	set_signal_handlers_prompt(); // set
	if (!isatty(0) || !isatty(1))
		return (0);
	while (TRUE)
	{
		command_list = NULL; token_list = NULL; line = NULL;
		if (g_tmp == 2 || g_tmp == 3) { // Ctrl+C at prompt OR Ctrl+C during heredoc
			data.last_exit_status = 130;
			g_tmp = 0;   
		}
		line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
		if (!line)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}

		if (line[0] == '\0') 
		{
			 free(line);
			 line = NULL;
			 continue;
		 }
		add_history(line);
		if (!ft_synax_error_free(line)) 
		{
			data.last_exit_status = 258;
			free(line); //DONE: FIXED Leak if error syntax
			line = NULL;
			continue; 
		}
		token_list = ft_tokenize(line);
		if (!token_list)
		{ 
			data.last_exit_status = 258;
			free(line);
			line = NULL;
			continue;
		}
		// ft_print_token_list(token_list);
		ft_expander(&token_list, &data);
		ft_print_token_list(token_list);
		command_list = ft_creat_cmd_table(token_list);
		ft_print_cmd_table(command_list);
		ft_token_clear(&token_list);
		if (command_list)
		{
			if (process_heredocs(command_list, &data) == EXIT_SUCCESS)
			{
				set_parent_wait_signal_handlers(&old_parent_sigint, &old_parent_sigquit); 
				g_tmp = 0;
				execute_commands(command_list, &data);
				cleanup_all_heredoc_fds(command_list);
				restore_signal_handlers(&old_parent_sigint, &old_parent_sigquit);
			}
			else if (g_tmp == 2)
			{
				data.last_exit_status = 130;
				g_tmp = 0;
				set_signal_handlers_prompt();
			}
			ft_cmd_clear(&command_list);
		}
		else
		{
			data.last_exit_status = 2;
			printf("[Command Table Creation Failed - Check Syntax]\n");
		}
		free(line);
		line = NULL;
	}
	ft_tenv_clear(&data.env_list);
	rl_clear_history();
	return (data. last_exit_status);
}
