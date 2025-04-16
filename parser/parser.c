/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:16:54 by eel-garo          #+#    #+#             */
/*   Updated: 2025/04/16 18:15:55 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	main(void)
{
	char	*line;
	t_token	*token_list_head;

	token_list_head = NULL;
	while (1)
	{
		line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
		if (!line)
			break ;
		token_list_head = NULL;
		if (line && *line && ft_synax_error_free(line))
		{
			add_history(line);
			token_list_head = ft_tokenize(line);
			if (token_list_head)
				ft_print_token_list(token_list_head);
		}
		free(line);
		ft_token_clear(&token_list_head);
		token_list_head = NULL;
		line = NULL;
	}
	rl_clear_history();
	return (0);
}
