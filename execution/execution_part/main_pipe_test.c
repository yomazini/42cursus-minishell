// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main_pipe_test.c                                   :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/04/24 20:34:29 by ymazini           #+#    #+#             */
// /*   Updated: 2025/04/29 20:07:30 by ymazini          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../exec_header.h"

// t_cmd *create_test_cmd_node(const char *args[])
// {
// 	t_cmd *cmd_node;
// 	int i = 0;
// 	int count = 0;

// 	while (args[count]) count++; // Count args

// 	cmd_node = malloc(sizeof(t_cmd));
// 	if (!cmd_node) return (perror("malloc cmd node failed"), NULL);

// 	cmd_node->argv = malloc(sizeof(char *) * (count + 1));
// 	if (!cmd_node->argv) {
//         perror("malloc argv failed");
//         free(cmd_node);
//         return (NULL);
//     }

// 	while (i < count) {
// 		cmd_node->argv[i] = ft_strdup(args[i]);
// 		if (!cmd_node->argv[i]) {
//             perror("strdup arg failed");
//             free_arr(cmd_node->argv);
//             free(cmd_node);
//             return (NULL);
//         }
// 		i++;
// 	}
// 	cmd_node->argv[i] = NULL;
// 	cmd_node->redirections = NULL;
// 	cmd_node->next = NULL;
// 	return (cmd_node);
// }

// void free_cmd_list(t_cmd *cmd_list) {
//     t_cmd *current = cmd_list;
//     t_cmd *next_cmd;
//     t_redir *redir, *next_redir;

//     while (current) {
//         next_cmd = current->next;
//         free_arr(current->argv);
//         redir = current->redirections;
//         while (redir) {
//             next_redir = redir->next;
//             free(redir->filename);
//             free(redir);
//             redir = next_redir;
//         }
//         free(current);
//         current = next_cmd;
//     }
// }

// int main(int ac, char **av, char **env)
// {
// 	(void)ac;
// 	(void)av;
// 	t_data	data;
// 	t_cmd	*cmd_list = NULL; // Head of our manual list
// 	t_cmd	*cmd1_node = NULL;
// 	t_cmd	*cmd2_node = NULL;

// 	// --- Initialize Data ---
// 	data.last_exit_status = EXIT_SUCCESS;
// 	data.env_list = ft_getenv(env);
// 	if (!data.env_list) {
//         ft_putstr_fd("Error initializing environment\n", 2);
//         return (1);
//     }

// 	// --- Manually Create Command List for: ls -l | wc -l ---
// 	printf("--- Testing Pipeline: ls -l | wc -l ---\n");

// 	// Create cmd1: ls -l
// 	const char *cmd1_args[] = {"ls", "-l", NULL};
// 	cmd1_node = create_test_cmd_node(cmd1_args);
// 	if (!cmd1_node) { ft_tenv_clear(&data.env_list); return (1); }

// 	// Create cmd2: wc -l
// 	const char *cmd2_args[] = {"wc", "-l", NULL};
// 	cmd2_node = create_test_cmd_node(cmd2_args);
// 	if (!cmd2_node) { free_cmd_list(cmd1_node); ft_tenv_clear(&data.env_list); return (1); }

// 	// Link them: cmd1 -> cmd2
// 	cmd1_node->next = cmd2_node;
// 	cmd_list = cmd1_node; // Head of the list is cmd1

// 	// --- Execute the Manually Created Pipeline ---
// 	if (cmd_list)
// 	{
// 		execute_commands(cmd_list, &data); // Call your main executor

// 		printf("[Pipeline execution finished with final status: %d]\n", data.last_exit_status);

// 		// Free the manually created list
// 		// free_cmd_list(cmd_list); // Assuming free_cmd_struct handles lists? If not, implement free_cmd_list
// 		cmd_list = NULL; // Avoid double free if execute_commands modifies list head somehow (it shouldn't)
// 	}

//     // --- Add more manual tests here if needed ---
//     // Example: cat < file | grep x > out
//     // You would need to manually create t_redir nodes as well for this test.

// 	// --- Final Cleanup ---
// 	ft_tenv_clear(&data.env_list);
// 	// rl_clear_history(); // Not using readline here

// 	printf("--- Test Complete ---\n");
// 	return (data.last_exit_status);
// }
