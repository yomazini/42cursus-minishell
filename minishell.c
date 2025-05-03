#include "includes/minishell.h" 
#include "parser/parser.h"
#include "execution/exec_header.h"

const char* get_token_type_name(t_token_type type) {
	switch (type) {
		case TOKEN_WORD:         return "WORD";
		case TOKEN_PIPE:         return "PIPE";
		case TOKEN_REDIR_IN:     return "REDIR_IN";
		case TOKEN_REDIR_OUT:    return "REDIR_OUT";
		case TOKEN_REDIR_APPEND: return "REDIR_APPEND"; // Make sure you have this enum
		case TOKEN_REDIR_HEREDOC:return "REDIR_HEREDOC";
		// Add cases for any other token types you might have
		default:                 return "UNKNOWN";
	}
}
void ft_print_token_list(t_token *head)
{
	t_token *current = head; 
	int i = 0;   
	while (current != NULL)
	{
		// Print in the requested format
		printf("token[%d]= [%s], type(%s)\n",
			   i,
			   current->value ? current->value : "(null value)", // Safety check
			   get_token_type_name(current->type)); // Get type name string

		// Move to the next node in the list
		current = current->next;
		i++; // Increment index
	}
}

int	is_simple_builtin_command(t_cmd *cmd)
{
	t_cmd	*current;
	char	*cmd_name;

	if (!cmd || cmd->argv[0] != TOKEN_WORD)
		return (FALSE);
	cmd_name = cmd->argv[0];
	current = cmd->next;
	while (current) {
		if (current->argv[0] != TOKEN_WORD) return (FALSE);
		current = current->next;
	}
	// Check against known builtins
	if ((ft_strncmp(cmd_name, "pwd", 4) == 0) ||
		(ft_strncmp(cmd_name, "echo", 5) == 0) || 
		(ft_strncmp(cmd_name, "env", 4) == 0) ||
		(ft_strncmp(cmd_name, "cd", 3) == 0) ||
		(ft_strncmp(cmd_name, "unset", 6) == 0) ||
		(ft_strncmp(cmd_name, "export", 7) == 0) ||
		(ft_strncmp(cmd_name, "exit", 5) == 0))
		return (TRUE);
	return (FALSE);
}


int	is_known_builtin(t_token *first_token) // Takes t_token*
{
	char	*cmd_name;

	// Checks if the token is valid and a WORD
	if (!first_token || first_token->type != TOKEN_WORD)
		return (FALSE);
	cmd_name = first_token->value;
	if (!cmd_name) return (FALSE); // Added safety check

	// Compares the value
	if ((ft_strncmp(cmd_name, "pwd", 4) == 0) ||
		(ft_strncmp(cmd_name, "echo", 5) == 0) ||
		(ft_strncmp(cmd_name, "env", 4) == 0) ||
		(ft_strncmp(cmd_name, "exit", 5) == 0) ||
        (ft_strncmp(cmd_name, "cd", 3) == 0) ||
        (ft_strncmp(cmd_name, "unset", 6) == 0) ||
        (ft_strncmp(cmd_name, "export", 7) == 0))
		return (TRUE);
	return (FALSE);
}

// int	contains_pipe(t_token *token_list)
// {
// 	t_token	*current = token_list;
// 	while (current)
// 	{
// 		if (current->type == TOKEN_PIPE)
// 			return (TRUE);
// 		current = current->next;
// 	}
// 	return (FALSE);
// }

// // static int	count_segment_words(t_token *token)
// // {
// // 	int count = 0;
// // 	while (token && token->type != TOKEN_PIPE)
// // 	{
// // 		if (token->type == TOKEN_WORD)
// // 			count++;
// // 		if (token->type >= TOKEN_REDIR_IN && token->type <= TOKEN_REDIR_HEREDOC)
// // 		{
// // 			if (token->next && token->next->type == TOKEN_WORD)
// // 				token = token->next;
// // 			else
// // 				break;
// // 		}
// // 		token = token->next;
// // 	}
// // 	return (count);
// // }

// // static char	**create_segment_argv(t_token *token, int word_count)
// // {
// // 	char	**argv_array;
// // 	int		i = 0;

// // 	if (word_count == 0) // Handle commands with only redirections
// // 		return (NULL); // Or maybe {"", NULL}? Let's return NULL for now.

// // 	argv_array = (char **)malloc(sizeof(char *) * (word_count + 1));
// // 	if (!argv_array) return (NULL);

// // 	while (token && token->type != TOKEN_PIPE)
// // 	{
// // 		if (token->type == TOKEN_WORD)
// // 		{
// // 			if (i < word_count) // Bounds check
// // 			{
// // 				argv_array[i] = ft_strdup(token->value);
// // 				if (!argv_array[i]) return (free_arr(argv_array), NULL);
// // 				i++;
// // 			} else break; // Should not happen if count was correct
// // 		}
// // 		else if (token->type >= TOKEN_REDIR_IN && token->type <= TOKEN_REDIR_HEREDOC)
// // 		{
// // 			if (token->next && token->next->type == TOKEN_WORD)
// // 				token = token->next; // Skip filename
// // 			else
// //                 break; // Error
// // 		}
// // 		token = token->next;
// // 	}
// // 	argv_array[i] = NULL;
// // 	return (argv_array);
// // }

// static t_redir *add_redir_node(t_redir **list_head, t_token *redir_token, t_token *filename_token)
// {
//     t_redir *new_redir = malloc(sizeof(t_redir));
// 	if (!new_redir) return (NULL);
// 	new_redir->type = redir_token->type;
// 	new_redir->filename = ft_strdup(filename_token->value);
// 	if (!new_redir->filename) return (free(new_redir), NULL);
// 	new_redir->heredoc_fd = -1;
// 	new_redir->expand_heredoc = true;
// 	new_redir->next = NULL;
// 	if (!*list_head) *list_head = new_redir;
// 	else {
// 		t_redir *curr = *list_head;
// 		while (curr->next) curr = curr->next;
// 		curr->next = new_redir;
// 	}
// 	return new_redir;
// }

// t_cmd	*placeholder_create_command_table(t_token *token_list)
// {
// 	t_cmd	*cmd_head = NULL;
// 	t_cmd	*current_cmd = NULL;
// 	t_token	*segment_start = token_list;
// 	t_token *current_token = token_list;

// 	if (!token_list) return (NULL);

// 	while (current_token != NULL)
// 	{
// 		while (current_token && current_token->type != TOKEN_PIPE)
// 			current_token = current_token->next;

// 		// Create a command node for the segment [segment_start ... current_token)
// 		t_cmd *new_cmd = malloc(sizeof(t_cmd));
// 		if (!new_cmd) { /* cleanup */ return (NULL); }
// 		new_cmd->redirections = NULL;
// 		new_cmd->next = NULL;

// 		// Process tokens in this segment to build argv and redirections
// 		t_token *seg_token = segment_start;
// 		int word_count = 0;
//         // First pass in segment: count words and build redirections
// 		while (seg_token != current_token)
// 		{
// 			if (seg_token->type == TOKEN_WORD) {
//                 word_count++;
//             } else if (seg_token->type >= TOKEN_REDIR_IN && seg_token->type <= TOKEN_REDIR_HEREDOC) {
//                  t_token *filename_token = seg_token->next;
//                  if (filename_token && filename_token->type == TOKEN_WORD && filename_token != current_token) {
//                      if (!add_redir_node(&new_cmd->redirections, seg_token, filename_token))
//                          { /* cleanup */ return (NULL); }
//                      seg_token = filename_token; // Skip filename
//                  } else { /* syntax error */ return (NULL); }
//             }
//             seg_token = seg_token->next;
// 		}

//         // Second pass: Build argv (needs careful indexing based on count)
//         new_cmd->argv = malloc(sizeof(char *) * (word_count + 1));
//         if (!new_cmd->argv) { /* cleanup */ return (NULL); }
//         seg_token = segment_start; // Reset for second pass
//         int argc = 0;
//         while (seg_token != current_token) {
//             if (seg_token->type == TOKEN_WORD) {
//                 if (argc < word_count) { // Bounds check
//                     new_cmd->argv[argc] = ft_strdup(seg_token->value);
//                     if (!new_cmd->argv[argc]) { /* cleanup */ return (NULL); }
//                     argc++;
//                 }
//             } else if (seg_token->type >= TOKEN_REDIR_IN && seg_token->type <= TOKEN_REDIR_HEREDOC) {
//                  // Skip redirection and filename again
//                  if (seg_token->next && seg_token->next->type == TOKEN_WORD)
//                     seg_token = seg_token->next;
//                  else break; // Error already handled, just break
//             }
//             seg_token = seg_token->next;
//         }
//         new_cmd->argv[argc] = NULL; // Null-terminate argv

// 		// Check if command is valid (must have a command if no redirections)
// 		if (argc == 0 && !new_cmd->redirections) {
//              ft_putstr_fd("minishell: syntax error near pipe or empty command\n", 2);
// 			 // free_cmd_list(cmd_head); // Need list free
// 			 free_cmd_struct(new_cmd); // Free invalid node
// 			 return NULL; // Invalid structure
// 		}

// 		// Link the new command node into the list
// 		if (!cmd_head) { // First command
// 			cmd_head = new_cmd;
// 			current_cmd = cmd_head;
// 		} else { // Subsequent commands
// 			current_cmd->next = new_cmd;
// 			current_cmd = new_cmd;
// 		}

// 		// Advance past the PIPE token for the next iteration
// 		if (current_token && current_token->type == TOKEN_PIPE)
// 		{
// 			segment_start = current_token->next; // Next segment starts after pipe
// 			current_token = current_token->next;
// 			// Check for pipe at end or consecutive pipes (parser should catch, but safety)
//             if (!segment_start || segment_start->type == TOKEN_PIPE) {
//                 ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
//                 // free_cmd_list(cmd_head);
//                 return NULL;
//             }
// 		}
// 	}
// 	return (cmd_head);
// }


// void	free_cmd_list(t_cmd *cmd_list) {
//     t_cmd *current_cmd = cmd_list;
//     t_cmd *next_cmd;
//     t_redir *redir, *next_redir;

//     while (current_cmd) {
//         next_cmd = current_cmd->next;
//         free_arr(current_cmd->argv);
//         redir = current_cmd->redirections;
//         while (redir) {
//             next_redir = redir->next;
//             free(redir->filename);
//             free(redir);
//             redir = next_redir;
//         }
//         free(current_cmd);
//         current_cmd = next_cmd;
//     }
// }

// int	main(int ac, char **av, char **env)
// {
// 	(void)ac;
// 	(void)av;
// 	t_data	data;
// 	char	*line;
// 	t_token	*token_list;
// 	t_cmd	*command_list;
	
// 	int status;
// 	// sleep(8); 
// 	// int fd = open("testgin.txt", O_CREAT | O_RDWR, 0644);
// 	// if (fd == 3)
// 	// 	printf("cool");
// 	// sleep(8); 
// 	// close(fd);
// 	line = NULL;
// 	token_list = NULL;
// 	command_list = NULL;
// 	data.last_exit_status = EXIT_SUCCESS;
// 	data.env_list = ft_getenv(env);

// 	if (!data.env_list && env && env[0]) {
//         ft_putstr_fd("Warning: Failed to initialize environment list.\n", 2);
//     }
// 	    update_shell_level(&data);
// 	while (TRUE)
// 	{
// 		command_list = NULL;
// 		// line = readline(">minishell$ :");
// 		line = readline("\001\033[1;36m\002>minishell$ \001\033[1;34m\002:\001\033[0m\002");
// 		if (!line)
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (line[0] == '\0')
//         {
//             free(line);
//             continue;
//         }
// 		add_history(line);
// 		// --- TODO: Add Mehdi's main syntax check here ---
// 		// if (!ft_syntax_error_free(line)) { free(line); line = NULL; data.last_exit_status = 2; continue; }
// 		// 1. Tokenize
// 		token_list = ft_tokenize(line);

// 		if (token_list)
// 		{
// 			// 2. Create Command Table/List (Using Placeholder)
// 			command_list = placeholder_create_command_table(token_list);
// 			if (command_list)
// 			{
//                 // --- FIX: Decision Logic ---
//                 // Check if the FIRST command in the list is a known builtin name
//                 // AND if there are NO pipes (i.e., it's a single command sequence)
// 				if (!command_list->next && is_known_builtin(token_list)) // Check first token and no pipes
// 				{
//                     // It's a single command AND a known builtin name
// 					status = execute_built_ins(command_list, &data); // Execute directly
//                     // Print status (adjusted logic)
//                     if (command_list->argv && command_list->argv[0] &&
//                         (ft_strncmp(command_list->argv[0], "exit", 5) != 0 || status != -1))
//                     {
//                         printf("[Builtin %s finished with status: %d]\n", \
//                                 command_list->argv[0], data.last_exit_status);
//                     }
// 				}
//                 else if (!command_list->next) // Single command, but NOT a builtin name
//                 {
//                     // It's a single external command
//                      status = execute_external_command(command_list, &data);
//                      printf("[External %s finished with status: %d]\n", \
//                              command_list->argv[0], data.last_exit_status);
//                 }
// 				else // It's a pipeline (command_list->next is not NULL)
// 				{
//                      status = execute_pipeline(command_list, &data);
//                      // Optional: Print status for pipeline
//                      // t_cmd *last_cmd = command_list;
//                      // while(last_cmd && last_cmd->next) last_cmd = last_cmd->next;
//                      // if (last_cmd && last_cmd->argv && last_cmd->argv[0]) {
//                      //     printf("[Pipeline ending with %s finished, final status: %d]\n",
//                      //            last_cmd->argv[0], data.last_exit_status);
//                      // }
// 				}
//                 // --- End FIX ---
// 				free_cmd_list(command_list); // Free the command list
// 				command_list = NULL;
// 			}
//             // ... (handle command table creation failure) ...
// 			ft_token_clear(&token_list); // Always clear tokens
// 			token_list = NULL;
// 		}
//         // ... (handle tokenization failure) ...
// 		free(line); // Free readline buffer
// 		line = NULL;
// 	}
// 	// Final cleanup
// 	ft_tenv_clear(&data.env_list);
// 	rl_clear_history();
// 	// printf("Final exit status: %d\n", data.last_exit_status); // Optional debug
// 	return (data.last_exit_status);
// }


// --- Global Signal Variable (If using this approach) ---
// volatile sig_atomic_t g_signal_status = 0;

// --- Signal Setup Function Prototype (Assume defined elsewhere) ---
// void	setup_signal_handlers(void);

int	main(int ac, char **av, char **env)
{
	t_data	data;          // Stores shell state
	char	*line;         // Input line from readline
	t_token	*token_list;   // Head of the token list from tokenizer
	t_cmd	*command_list; // Head of the command table from parser

	(void)ac; // Suppress unused parameter warnings
	(void)av; // Suppress unused parameter warnings
	// printf("Testing 1.0\n");
	// printf("Testing 2.0\n");
	// --- Initialization ---
	line = NULL;
	token_list = NULL;
	command_list = NULL;
	data.last_exit_status = EXIT_SUCCESS; // Initialize last status to 0
	// printf("Testing 3.0\n");
	data.env_list = ft_getenv(env);       // Parse initial environment (Mehdi's func)
	if (!data.env_list && env && env[0])
		ft_putstr_fd("minishell: Warning: env list init failed.\n", 2);
	update_shell_level(&data);             // Update SHLVL (Youssef's func)
	// printf("Testing 4.0\n");
	// setup_signal_handlers();            // Initialize signal handlers

	// --- Main Read-Execute-Print Loop (REPL) ---
	while (TRUE)
	{
		command_list = NULL; // Reset pointers for this iteration
		token_list = NULL;
		line = NULL;
		// printf("Testing 5.0\n");

		// --- TODO: Check global signal status if using signals ---
		// if (g_signal_status == SIGINT) { ... handle prompt interrupt ... }

		// 1. Read Line
		line = readline("\001\033[1;36m\002minishell$ \001\033[1;34m\002:\001\033[0m\002 "); // Prompt

		// 2. Handle EOF (Ctrl+D)
		if (!line)
		{
			// ft_putstr_fd("exit\n", STDOUT_FILENO); // Mimic bash output
			// write(1, "exit", 5);
			break ; // Exit the main loop
		}

		// printf("Testing 6.0\n");
		// 3. Handle Empty Input & History
		if (line[0] == '\0')
		{
			free(line);
			continue; // Skip rest of loop, get new prompt
		}
		add_history(line); // Add non-empty line to history

		// --- Parsing Pipeline ---

		// 4. Initial Syntax Check (Mehdi's function)
		if (!ft_synax_error_free(line)) // Check basic syntax on raw line
		{
			// printf("Testing 6.0\n");
			data.last_exit_status = 2; // Syntax error status
			free(line);                // Free line buffer
			continue;                  // Skip to next prompt
		}

		// 5. Tokenize (Mehdi's function)
		token_list = ft_tokenize(line);
			// printf("Testing 6.0\n");
		if (!token_list) // Tokenizer failed (e.g., malloc error, or maybe syntax error caught here?)
		{
			// printf("Testing 6.0\n");
			data.last_exit_status = 2; // Assume syntax or malloc error
			// Tokenizer or syntax check should have printed error
			free(line);
			continue;
		}

		// 6. Expand Variables & Remove Quotes (Mehdi's function)
		// Ensure ft_expander handles quote removal or call ft_clean_up after.
		ft_expander(&token_list, &data /*, data.last_exit_status ? */);
		// ft_print_token_list(token_list);
			// printf("Testing 6.0\n");
		// ft_clean_up(&token_list); // If quote removal is separate

		// 7. Create Command Table (Mehdi's function)
		command_list = ft_creat_cmd_table(token_list);
		// We are done with the token list now, free it.
		ft_token_clear(&token_list); // Use Mehdi's token clearer
		token_list = NULL;           // Reset pointer

		// --- Execution Phase ---
		if (command_list) // Check if command table creation was successful
		{
			// printf("Testing 8.0\n");
			// --- TODO: Heredoc Processing Step (Youssef's function) ---
			// Needs to iterate command_list and handle all << redirections,
			// reading input and storing readable FDs in t_redir->heredoc_fd.
			// Must be done BEFORE execute_commands.
			// if (process_heredocs(command_list, &data) != SUCCESS) {
			//     data.last_exit_status = ...;
			// } else { // Only execute if heredocs ok

			// 8. Execute the command structure (Youssef's main entry point)
			execute_commands(command_list, &data); // Handles single/pipe/builtin/external
			// printf("Testing 9.0\n");

			// } // End else block for heredoc success

			// 9. Cleanup Command Table for this line
			ft_cmd_clear(&command_list); // Use Mehdi's command table clearer
			command_list = NULL;
		}
		else // Command table creation failed
		{
			data.last_exit_status = 2; // Assume syntax error from builder
			printf("[Command Table Creation Failed - Check Syntax]\n"); // Debug
		}

		// 10. Cleanup Readline Buffer
		free(line);
		line = NULL;

	} // --- End of main while loop ---

	// --- Final Cleanup ---
	ft_tenv_clear(&data.env_list); // Free environment list (Mehdi's func)
	// rl_clear_history(); // Clear readline history

	// printf("Final exit status: %d\n", data.last_exit_status); // Optional debug
	return (data.last_exit_status); // Return the last exit status
}




