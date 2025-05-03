/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:16:54 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/02 13:11:30 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
// int	main(int ac, char **av, char **env)
// {
// 	char	*line;
// 	t_token	*token_list_head;
// 	t_env	*env_list_head;
// 	t_cmd	*cmd_list_head;

// 	token_list_head = NULL;
// 	env_list_head = NULL;
// 	cmd_list_head = NULL;
// 	(void)ac;
// 	(void)av;
// 	env_list_head = ft_getenv(env);
// 	//~ one-time setup; ensures the pointer is not holding a garbage value
// 	while (1)
// 	{
// 		//* FILL env Likend list here
// 		line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
// 		if (!line)
// 			break ;
// 		token_list_head = NULL;
// 		//~ init = NULL per-command reset; after each tokenize list
// 		if (line && *line && ft_synax_error_free(line))
// 		{
// 			add_history(line);
// 			token_list_head = ft_tokenize(line);
// 			ft_expander(&token_list_head, env_list_head);
// 			cmd_list_head =  ft_creat_cmd_table(token_list_head);
// 			ft_print_cmd_table(cmd_list_head);
// 			if (token_list_head)
// 				ft_print_token_list(token_list_head);
// 		}
		
// 		free(line);
// 		ft_token_clear(&token_list_head);
// 		ft_cmd_clear(&cmd_list_head);
// 		cmd_list_head = NULL;
// 		token_list_head = NULL;
// 		line = NULL;
// 	}
// 	free(line);
// 	line = NULL;
// 	ft_tenv_clear(&env_list_head);
// 	rl_clear_history();
// 	return (0);
// }
