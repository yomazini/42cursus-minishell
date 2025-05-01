/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:45:09 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/01 22:27:12 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_header.h" 

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

#include "exec_header.h"

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

int	contains_pipe(t_token *token_list)
{
	t_token	*current = token_list;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			return (TRUE);
		current = current->next;
	}
	return (FALSE);
}

// static int	count_segment_words(t_token *token)
// {
// 	int count = 0;
// 	while (token && token->type != TOKEN_PIPE)
// 	{
// 		if (token->type == TOKEN_WORD)
// 			count++;
// 		if (token->type >= TOKEN_REDIR_IN && token->type <= TOKEN_REDIR_HEREDOC)
// 		{
// 			if (token->next && token->next->type == TOKEN_WORD)
// 				token = token->next;
// 			else
// 				break;
// 		}
// 		token = token->next;
// 	}
// 	return (count);
// }

// static char	**create_segment_argv(t_token *token, int word_count)
// {
// 	char	**argv_array;
// 	int		i = 0;

// 	if (word_count == 0) // Handle commands with only redirections
// 		return (NULL); // Or maybe {"", NULL}? Let's return NULL for now.

// 	argv_array = (char **)malloc(sizeof(char *) * (word_count + 1));
// 	if (!argv_array) return (NULL);

// 	while (token && token->type != TOKEN_PIPE)
// 	{
// 		if (token->type == TOKEN_WORD)
// 		{
// 			if (i < word_count) // Bounds check
// 			{
// 				argv_array[i] = ft_strdup(token->value);
// 				if (!argv_array[i]) return (free_arr(argv_array), NULL);
// 				i++;
// 			} else break; // Should not happen if count was correct
// 		}
// 		else if (token->type >= TOKEN_REDIR_IN && token->type <= TOKEN_REDIR_HEREDOC)
// 		{
// 			if (token->next && token->next->type == TOKEN_WORD)
// 				token = token->next; // Skip filename
// 			else
//                 break; // Error
// 		}
// 		token = token->next;
// 	}
// 	argv_array[i] = NULL;
// 	return (argv_array);
// }

static t_redir *add_redir_node(t_redir **list_head, t_token *redir_token, t_token *filename_token)
{
    t_redir *new_redir = malloc(sizeof(t_redir));
	if (!new_redir) return (NULL);
	new_redir->type = redir_token->type;
	new_redir->filename = ft_strdup(filename_token->value);
	if (!new_redir->filename) return (free(new_redir), NULL);
	new_redir->heredoc_fd = -1;
	new_redir->expand_heredoc = true;
	new_redir->next = NULL;
	if (!*list_head) *list_head = new_redir;
	else {
		t_redir *curr = *list_head;
		while (curr->next) curr = curr->next;
		curr->next = new_redir;
	}
	return new_redir;
}

t_cmd	*placeholder_create_command_table(t_token *token_list)
{
	t_cmd	*cmd_head = NULL;
	t_cmd	*current_cmd = NULL;
	t_token	*segment_start = token_list;
	t_token *current_token = token_list;

	if (!token_list) return (NULL);

	while (current_token != NULL)
	{
		while (current_token && current_token->type != TOKEN_PIPE)
			current_token = current_token->next;

		// Create a command node for the segment [segment_start ... current_token)
		t_cmd *new_cmd = malloc(sizeof(t_cmd));
		if (!new_cmd) { /* cleanup */ return (NULL); }
		new_cmd->redirections = NULL;
		new_cmd->next = NULL;

		// Process tokens in this segment to build argv and redirections
		t_token *seg_token = segment_start;
		int word_count = 0;
        // First pass in segment: count words and build redirections
		while (seg_token != current_token)
		{
			if (seg_token->type == TOKEN_WORD) {
                word_count++;
            } else if (seg_token->type >= TOKEN_REDIR_IN && seg_token->type <= TOKEN_REDIR_HEREDOC) {
                 t_token *filename_token = seg_token->next;
                 if (filename_token && filename_token->type == TOKEN_WORD && filename_token != current_token) {
                     if (!add_redir_node(&new_cmd->redirections, seg_token, filename_token))
                         { /* cleanup */ return (NULL); }
                     seg_token = filename_token; // Skip filename
                 } else { /* syntax error */ return (NULL); }
            }
            seg_token = seg_token->next;
		}

        // Second pass: Build argv (needs careful indexing based on count)
        new_cmd->argv = malloc(sizeof(char *) * (word_count + 1));
        if (!new_cmd->argv) { /* cleanup */ return (NULL); }
        seg_token = segment_start; // Reset for second pass
        int argc = 0;
        while (seg_token != current_token) {
            if (seg_token->type == TOKEN_WORD) {
                if (argc < word_count) { // Bounds check
                    new_cmd->argv[argc] = ft_strdup(seg_token->value);
                    if (!new_cmd->argv[argc]) { /* cleanup */ return (NULL); }
                    argc++;
                }
            } else if (seg_token->type >= TOKEN_REDIR_IN && seg_token->type <= TOKEN_REDIR_HEREDOC) {
                 // Skip redirection and filename again
                 if (seg_token->next && seg_token->next->type == TOKEN_WORD)
                    seg_token = seg_token->next;
                 else break; // Error already handled, just break
            }
            seg_token = seg_token->next;
        }
        new_cmd->argv[argc] = NULL; // Null-terminate argv

		// Check if command is valid (must have a command if no redirections)
		if (argc == 0 && !new_cmd->redirections) {
             ft_putstr_fd("minishell: syntax error near pipe or empty command\n", 2);
			 // free_cmd_list(cmd_head); // Need list free
			 free_cmd_struct(new_cmd); // Free invalid node
			 return NULL; // Invalid structure
		}

		// Link the new command node into the list
		if (!cmd_head) { // First command
			cmd_head = new_cmd;
			current_cmd = cmd_head;
		} else { // Subsequent commands
			current_cmd->next = new_cmd;
			current_cmd = new_cmd;
		}

		// Advance past the PIPE token for the next iteration
		if (current_token && current_token->type == TOKEN_PIPE)
		{
			segment_start = current_token->next; // Next segment starts after pipe
			current_token = current_token->next;
			// Check for pipe at end or consecutive pipes (parser should catch, but safety)
            if (!segment_start || segment_start->type == TOKEN_PIPE) {
                ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
                // free_cmd_list(cmd_head);
                return NULL;
            }
		}
	}
	return (cmd_head);
}


void	free_cmd_list(t_cmd *cmd_list) {
    t_cmd *current_cmd = cmd_list;
    t_cmd *next_cmd;
    t_redir *redir, *next_redir;

    while (current_cmd) {
        next_cmd = current_cmd->next;
        free_arr(current_cmd->argv);
        redir = current_cmd->redirections;
        while (redir) {
            next_redir = redir->next;
            free(redir->filename);
            free(redir);
            redir = next_redir;
        }
        free(current_cmd);
        current_cmd = next_cmd;
    }
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_data	data;
	char	*line;
	t_token	*token_list;
	t_cmd	*command_list;
	
	int status;
	// sleep(8); 
	// int fd = open("testgin.txt", O_CREAT | O_RDWR, 0644);
	// if (fd == 3)
	// 	printf("cool");
	// sleep(8); 
	// close(fd);
	line = NULL;
	token_list = NULL;
	command_list = NULL;
	data.last_exit_status = EXIT_SUCCESS;
	data.env_list = ft_getenv(env);

	if (!data.env_list && env && env[0]) {
        ft_putstr_fd("Warning: Failed to initialize environment list.\n", 2);
    }
	    update_shell_level(&data);
	while (TRUE)
	{
		command_list = NULL;
		// line = readline(">minishell$ :");
		line = readline("\001\033[1;36m\002>minishell$ \001\033[1;34m\002:\001\033[0m\002");
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
		// --- TODO: Add Mehdi's main syntax check here ---
		// if (!ft_syntax_error_free(line)) { free(line); line = NULL; data.last_exit_status = 2; continue; }
		// 1. Tokenize
		token_list = ft_tokenize(line);

		if (token_list)
		{
			// 2. Create Command Table/List (Using Placeholder)
			command_list = placeholder_create_command_table(token_list);
			if (command_list)
			{
                // --- FIX: Decision Logic ---
                // Check if the FIRST command in the list is a known builtin name
                // AND if there are NO pipes (i.e., it's a single command sequence)
				if (!command_list->next && is_known_builtin(token_list)) // Check first token and no pipes
				{
                    // It's a single command AND a known builtin name
					status = execute_built_ins(command_list, &data); // Execute directly
                    // Print status (adjusted logic)
                    if (command_list->argv && command_list->argv[0] &&
                        (ft_strncmp(command_list->argv[0], "exit", 5) != 0 || status != -1))
                    {
                        printf("[Builtin %s finished with status: %d]\n", \
                                command_list->argv[0], data.last_exit_status);
                    }
				}
                else if (!command_list->next) // Single command, but NOT a builtin name
                {
                    // It's a single external command
                     status = execute_external_command(command_list, &data);
                     printf("[External %s finished with status: %d]\n", \
                             command_list->argv[0], data.last_exit_status);
                }
				else // It's a pipeline (command_list->next is not NULL)
				{
                     status = execute_pipeline(command_list, &data);
                     // Optional: Print status for pipeline
                     // t_cmd *last_cmd = command_list;
                     // while(last_cmd && last_cmd->next) last_cmd = last_cmd->next;
                     // if (last_cmd && last_cmd->argv && last_cmd->argv[0]) {
                     //     printf("[Pipeline ending with %s finished, final status: %d]\n",
                     //            last_cmd->argv[0], data.last_exit_status);
                     // }
				}
                // --- End FIX ---
				free_cmd_list(command_list); // Free the command list
				command_list = NULL;
			}
            // ... (handle command table creation failure) ...
			ft_token_clear(&token_list); // Always clear tokens
			token_list = NULL;
		}
        // ... (handle tokenization failure) ...
		free(line); // Free readline buffer
		line = NULL;
	}
	// Final cleanup
	ft_tenv_clear(&data.env_list);
	rl_clear_history();
	// printf("Final exit status: %d\n", data.last_exit_status); // Optional debug
	return (data.last_exit_status);
}
