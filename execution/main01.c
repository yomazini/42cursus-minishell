/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:45:09 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/19 15:44:52 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_header.h"
 
//TODO: this is for testing my execution part
 
int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av; 
	t_data data; 
	data.envp = env;
	data.last_exit_status = EXIT_SUCCESS;
	char *line = NULL;
	char **args = NULL;
	int status;
	while(TRUE)
	{
		line = readline(">testing built_in:");
		if (!line)
		{
			printf("exit\n");
			break;
		}	
		if (line[0])
			add_history(line);
		args = ft_split(line,' ');
		if (!args)
		{
			perror("ft_split failed:");
			data.last_exit_status = EXIT_FAILURE;
			free(line);
			continue; 
		}	
		if (args[0])
		{
			t_cmd test_cmd;
			test_cmd.argv = args;
			status = execute_built_ins( &test_cmd , &data);
			if (status != -1 )
				printf("\nbuilt_in was executed with %d\n", data.last_exit_status);
			else
			{
				printf("minishell test: command not found: %s\n", args[0]);
				data.last_exit_status = 127;
			}
		}
		else
			data.last_exit_status = EXIT_SUCCESS;
		free_arr(args);	
		args = NULL;
		free(line);
		line = NULL;
	}
	return (data.last_exit_status);
}


// TODO: this is for Combine Exec and Parse
