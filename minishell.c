#include "includes/minishell.h" 
#include "parser/parser.h"
#include "execution/exec_header.h"
#include <signal.h>

int g_child_open = 0;

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
void	ft_handler(int signum)
{
		if (signum == SIGINT)
		{
			if (!g_child_open)
			{
				write(1, "\n",1);
				rl_on_new_line();
				rl_replace_line("", 0);
				rl_redisplay();
			}
			else if (g_child_open == 1)
			{
				g_child_open = 2;
				write(1, "\n", 1);
			}
		}
		if (signum == SIGQUIT)
		{
			if (g_child_open == 1)
			printf("Quit: 3\n");
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


// TODO: Here must edit the ctl + C to be 130 in last status {128 + 2} --> update last exit status;

int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*line;
	t_token	*token_list;
	t_cmd	*command_list;
	

	(void)ac; //~ maby add in a check: if ac > 1 -> do all of this 
	(void)av;
	line = NULL;
	token_list = NULL;
	command_list = NULL;
	data.last_exit_status = EXIT_SUCCESS;
	data.env_list = ft_getenv(env);
	rl_catch_signals = 0;
	if (!data.env_list && env && env[0])
		ft_putstr_fd("minishell: Warning: env list init failed.\n", 2);
	update_shell_level(&data);
	signal(SIGINT, ft_handler);
	signal(SIGQUIT, ft_handler);
	while (TRUE)
	{
		// command_list = NULL;
		// token_list = NULL;
		// line = NULL;
		line = readline(MINISHELL_PROMPT);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue;
		}
		add_history(line);
		if (!ft_synax_error_free(line))
		{
			data.last_exit_status = 258;
			free(line);
			continue;
		}
		token_list = ft_tokenize(line);
		if (!token_list)
		{
			data.last_exit_status = 258;
			free(line);
			continue;
		}
		ft_expander(&token_list, &data);
		// ft_print_token_list(token_list);
		command_list = ft_creat_cmd_table(token_list);
		// ft_print_cmd_table(command_list);
		ft_token_clear(&token_list);
		token_list = NULL;
		if (command_list)
		{
			// --- *** NEW: Process Heredocs BEFORE Execution *** ---
			if (process_heredocs(command_list, &data) == EXIT_SUCCESS)
			{
				// Heredocs processed ok (or none found), proceed to execute
				execute_commands(command_list, &data);
			}
			else
			{
				// Heredoc processing failed (e.g., Ctrl+D, file error)
				// Status is already set in data by process_heredocs
				printf("[Heredoc processing failed or interrupted]\n");
			}
			// if (process_heredocs(command_list, &data))
			// execute_commands(command_list, &data);
			ft_cmd_clear(&command_list);
			command_list = NULL;
		}
		else
		{
			data.last_exit_status = 2; // should it be 2 or 258 ?? Why ? and How ?
			printf("[Command Table Creation Failed - Check Syntax]\n");
			// here free command_list linked list
		}
		free(line);
		line = NULL;
	}
	cleanup_shell_resources(&data);
	// ft_tenv_clear(&data.env_list);
	// rl_clear_history();
	return (data.last_exit_status);
}
