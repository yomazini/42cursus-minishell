/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isvariablet_exist.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:04:51 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/10 16:09:39 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

char	*ft_isvariablet_exist(t_env *env_list, char *variable_name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (current->name && ft_strcmp(current->name, variable_name) == 0)
		{
			if (current->value)
				return (ft_strdup(current->value));
		}
		current = current->next;
	}
	return (NULL);
}
