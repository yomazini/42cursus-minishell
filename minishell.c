#include "includes/minishell.h" 
#include "parser/parser.h"
#include "execution/exec_header.h"
#include <signal.h>

// const char* get_token_type_name(t_token_type type) {
// 	switch (type) {
// 		case TOKEN_WORD:         return "WORD";
// 		case TOKEN_PIPE:         return "PIPE";
// 		case TOKEN_REDIR_IN:     return "REDIR_IN";
// 		case TOKEN_REDIR_OUT:    return "REDIR_OUT";
// 		case TOKEN_REDIR_APPEND: return "REDIR_APPEND";
// 		case TOKEN_REDIR_HEREDOC:return "REDIR_HEREDOC";
// 		default:                 return "UNKNOWN";
// 	}
// }
// void ft_print_token_list(t_token *head)
// {
// 	t_token *current = head; 
// 	int i = 0;   
// 	while (current != NULL)
// 	{
// 		printf("token[%d]= [%s], type(%s)\n",
// 			   i,
// 			   current->value ? current->value : "(null value)",
// 			   get_token_type_name(current->type));
// 		current = current->next;
// 		i++;
// 	}
// }

void	handler_sigint(int signum)
{
		(void)signum;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
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
	if (!data.env_list && env && env[0])
		ft_putstr_fd("minishell: Warning: env list init failed.\n", 2);
	update_shell_level(&data);
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (TRUE)
	{
		// command_list = NULL;
		// token_list = NULL;
		// line = NULL;
		line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
		if (!line)
		{
			printf("exit\n");// here better to do write as long as like bash {pay attention if can reditrect or not if like /dev/stdout or fd}
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
		command_list = ft_creat_cmd_table(token_list);
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
	ft_tenv_clear(&data.env_list);
	rl_clear_history();
	return (data.last_exit_status);
}


// --> in mainbsb
// int	main(int ac, char **av, char **env)
// {
// 	// ... (declarations, initialization, update_shell_level) ...

// 	while (TRUE)
// 	{
// 		// ... (reset pointers, readline, EOF check, empty check, history) ...

// 		// --- Parsing Pipeline ---
// 		// ... (syntax check, tokenize) ...

// 		if (token_list)
// 		{
// 			// ... (expander, quote removal) ...
// 			command_list = ft_create_cmd_table(token_list);
// 			ft_token_clear(&token_list); // Clear tokens early

// 			if (command_list)
// 			{
// 				// --- *** NEW: Process Heredocs BEFORE Execution *** ---
// 				if (process_heredocs(command_list, &data) == EXIT_SUCCESS)
// 				{
// 					// Heredocs processed ok (or none found), proceed to execute
// 					execute_commands(command_list, &data);
// 				}
// 				else
// 				{
// 					// Heredoc processing failed (e.g., Ctrl+D, file error)
// 					// Status is already set in data by process_heredocs
// 					printf("[Heredoc processing failed or interrupted]\n");
// 				}
// 				// --- *** END HEREDOC PROCESSING *** ---

// 				ft_cmd_clear(&command_list); // Always clear cmd list
// 				command_list = NULL;
// 			}
// 			// ... (handle command table creation failure) ...
// 		}
//         // ... (handle tokenization failure) ...

// 		// ... (free line) ...
// 	} // End while loop

// 	// ... (final cleanup) ...
// 	return (data.last_exit_status);
// }
