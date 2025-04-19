/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:45:09 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/19 16:00:06 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_header.h"
 
//TODO: this is for testing my execution part
 
// int main(int ac, char **av, char **env)
// {
// 	(void)ac;
// 	(void)av;
// 	t_data data; 
// 	data.envp = env;
// 	data.last_exit_status = EXIT_SUCCESS;
// 	char *line = NULL;
// 	char **args = NULL;
// 	int status;
// 	while(TRUE)
// 	{
// 		line = readline(">testing built_in:");
// 		if (!line)
// 		{
// 			printf("exit\n");
// 			break;
// 		}	
// 		if (line[0])
// 			add_history(line);
// 		args = ft_split(line,' ');
// 		if (!args)
// 		{
// 			perror("ft_split failed:");
// 			data.last_exit_status = EXIT_FAILURE;
// 			free(line);
// 			continue; 
// 		}	
// 		if (args[0])
// 		{
// 			t_cmd test_cmd;
// 			test_cmd.argv = args;
// 			status = execute_built_ins( &test_cmd , &data);
// 			if (status != -1 )
// 				printf("\nbuilt_in was executed with %d\n", data.last_exit_status);
// 			else
// 			{
// 				printf("minishell test: command not found: %s\n", args[0]);
// 				data.last_exit_status = 127;
// 			}
// 		}
// 		else
// 			data.last_exit_status = EXIT_SUCCESS;
// 		free_arr(args);	
// 		args = NULL;
// 		free(line);
// 		line = NULL;
// 	}
// 	return (data.last_exit_status);
// }

// TODO: this is for Combine Exec and Parse

// Helper to check if a token list represents a simple, single builtin command
// Returns TRUE if simple builtin, FALSE otherwise
int	is_simple_builtin_command(t_token *token_list)
{
	t_token *current;
	char	*cmd_name;

	if (!token_list || token_list->type != TOKEN_WORD)
		return (FALSE); // Must start with a command word

	cmd_name = token_list->value;
	current = token_list->next;

	// Check for pipes or redirections anywhere in the list
	while (current)
	{
		if (current->type == TOKEN_PIPE ||
			current->type == TOKEN_REDIR_IN ||
			current->type == TOKEN_REDIR_OUT ||
			current->type == TOKEN_REDIR_APPEND ||
			current->type == TOKEN_REDIR_HEREDOC)
		{
			return (FALSE); // Contains pipe or redirection, not simple
		}
		current = current->next;
	}

	// Check if the first word is a known builtin
	if ((ft_strncmp(cmd_name, "pwd", 4) == 0) ||
		(ft_strncmp(cmd_name, "echo", 5) == 0) ||
		(ft_strncmp(cmd_name, "exit", 5) == 0))
		// Add other builtins here (env, cd, export, unset) if ready
	{
		return (TRUE);
	}

	return (FALSE); // Not a simple builtin
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_data data;
	data.envp = env; // Keep using envp for now
	data.last_exit_status = EXIT_SUCCESS;
	char *line = NULL;
	t_token *token_list = NULL;
	t_cmd   *command = NULL;
	int status_code; // To store return from builtins if needed

	while (TRUE)
	{
		line = readline("\001\033[1;36m\002>minishell$ \001\033[1;34m\002:\001\033[0m\002 ");
		if (!line)
		{
			printf("exit\n"); // Handle Ctrl+D
			break;
		}
		if (line[0])
			add_history(line);
		// 1. Tokenize (Mehdi's part)
		// Assume ft_tokenize handles basic syntax errors and returns NULL
		token_list = ft_tokenize(line);
		if (token_list)
		{
			// 2. Check if it's a simple builtin we can execute
			if (is_simple_builtin_command(token_list))
			{
				// 3. Convert simple tokens to t_cmd struct (Your temporary bridge)
				command = convert_simple_tokens_to_cmd(token_list);
				if (command)
				{
					// 4. Execute the builtin
					status_code = execute_built_ins(command, &data);
                    // execute_built_ins's return is the status. ft_exit won't return here.
                    // data.last_exit_status is updated inside the builtins.
                    if (ft_strncmp(command->argv[0], "exit", 5) != 0 || command->argv[1] != NULL)
                         printf("[Builtin %s finished with status: %d]\n", \
                                 command->argv[0], data.last_exit_status);
					free_cmd_struct(command); // Free the temporary cmd struct
					command = NULL;
				}
				else
				{
					perror("Failed to convert tokens to command");
					data.last_exit_status = EXIT_FAILURE;
				}
			}
			else // Not a simple builtin (has pipes/redirects or isn't pwd/echo/exit)
			{
				printf("--> Complex command or not a builtin, printing tokens:\n");
				ft_print_token_list(token_list); // Use Mehdi's print function
				// Decide on a default status? Or wait for executor? Let's set 0 for now.
				// data.last_exit_status = 0;
			}
			// 5. Clean up token list from tokenizer
			ft_token_clear(&token_list);
			token_list = NULL;
		}
		else // Tokenizer returned NULL (e.g., syntax error)
		{
			data.last_exit_status = 2; // Common syntax error status
			printf("[Syntax Error or Tokenization Failed]\n");
		}

		// 6. Clean up readline buffer
		free(line);
		line = NULL;
	}
	// rl_clear_history(); // Optional
	return (data.last_exit_status);
}