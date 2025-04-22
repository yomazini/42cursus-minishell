/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:45:09 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/21 15:31:46 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_header.h" 

int	is_simple_builtin_command(t_token *token_list)
{
    t_token	*current;
    char	*cmd_name;

    if (!token_list || token_list->type != TOKEN_WORD)
        return (FALSE);
    cmd_name = token_list->value;
    current = token_list->next;
    while (current) { // Check for pipes/redirects
        if (current->type != TOKEN_WORD) 
			return (FALSE);
		current = current->next;
    }
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

static char	**create_argv_from_simple_tokens(t_token *token_list)
{
    int		count = 0;
    t_token	*curr = token_list;
    char	**argv_array;
    int		i = 0;

    while (curr && curr->type == TOKEN_WORD) 
	{
        count++;
        curr = curr->next;
    }
    argv_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!argv_array) 
		return (NULL);
    curr = token_list;
    while (curr && curr->type == TOKEN_WORD && i < count) {
        argv_array[i] = ft_strdup(curr->value);
        if (!argv_array[i]) 
			return (free_arr(argv_array), NULL);
        i++;
        curr = curr->next;
    }
    argv_array[i] = NULL;
    return (argv_array);
}

t_cmd	*convert_simple_tokens_to_cmd(t_token *token_list)
{
    t_cmd	*new_cmd;

    if (!token_list) 
		return (NULL);
    new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (!new_cmd) 
		return (NULL);
    new_cmd->redirections = NULL;
    new_cmd->next = NULL;
    new_cmd->argv = create_argv_from_simple_tokens(token_list);
    if (!new_cmd->argv) 
		return (free(new_cmd), NULL);
    return (new_cmd);
}

int	main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    t_data	data;
    char	*line;
    t_token	*token_list;
    t_cmd	*command;
    int		status;

    line = NULL;
    token_list = NULL;
    command = NULL;
    data.last_exit_status = EXIT_SUCCESS;
    data.env_list = ft_getenv(env);
    while (TRUE)
    {
        line = readline("\001\033[1;36m\002>minishell$ \001\033[1;34m\002:\001\033[0m\002 ");
        if (!line)
        {
            printf("exit\n");
            break ;
        }
        if (line[0])
            add_history(line);
         token_list = ft_tokenize(line);
        if (token_list)
        {
            if (is_simple_builtin_command(token_list))
            {
                command = convert_simple_tokens_to_cmd(token_list);
                if (command)
                {
                    status = execute_built_ins(command, &data);
                     if (command->argv && command->argv[0] &&
                        (ft_strncmp(command->argv[0], "exit", 5) != 0 || status != -1))
                     {
                        printf("[Builtin %s finished with status: %d]\n", \
                                command->argv[0], data.last_exit_status);
                     }
                    free_cmd_struct(command);
                    command = NULL;
                }
                else // Conversion failed
                {
                    perror("Failed to convert tokens to command");
                    data.last_exit_status = EXIT_FAILURE;
                }
            }
            else
            {
                printf("--> Complex command or not a simple builtin, printing tokens:\n");
                ft_print_token_list(token_list);
            }
            ft_token_clear(&token_list);
            token_list = NULL;
        }
        else
        {
            data.last_exit_status = 2;
            printf("[Syntax Error or Tokenization Failed]\n");
        }
        free(line);
        line = NULL;
    }
    ft_tenv_clear(&data.env_list);
    return (data.last_exit_status);
}
