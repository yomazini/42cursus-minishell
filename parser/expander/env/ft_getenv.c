/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:11:25 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 13:13:36 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../parser.h"

static char	**fill_env(char **sp_env, char *env_line, char *eq_ptr)
{
	size_t	name_lenght;

	if (!eq_ptr)
	{
		sp_env[0] = ft_strdup(env_line);
		sp_env[1] = ft_strdup("");
	}
	else if (eq_ptr)
	{
		name_lenght = eq_ptr - env_line;
		sp_env[0] = malloc(name_lenght + 1);
		if (sp_env[0])
		{
			ft_strncpy(sp_env[0], env_line, name_lenght);
			sp_env[0][name_lenght] = '\0';
		}
		sp_env[1] = ft_strdup(eq_ptr + 1);
	}
	if (!sp_env[0] || !sp_env[1])
		return (free(sp_env[0]), free(sp_env[1]), free(sp_env), NULL);
	sp_env[2] = NULL;
	return (sp_env);
}

static char	**ft_split_env(char *env_line)
{
	char	**sp_env;
	char	*eq_ptr;

	eq_ptr = ft_strchr(env_line, '=');
	sp_env = malloc(sizeof(char *) * (2 + 1));
	if (!sp_env)
		return (NULL);
	if (!fill_env(sp_env, env_line, eq_ptr))
		return (NULL);
	return (sp_env);
}

int	ft_flag_env(char *env_line)
{
	if (env_line[0] == '_')
		return (1);
	else if (ft_strnstr(env_line, "SHLVL", ft_strlen(env_line)))
		return (1);
	return (0);
}

t_env	*ft_getenv(char **env)
{
	int		i;
	char	**sp_env;
	t_env	*head;
	t_env	*new_node;
	int		flag;

	head = NULL;
	new_node = NULL;
	i = 0;
	while (env && env[i])
	{
		sp_env = ft_split_env(env[i]);
		if (sp_env && sp_env[0] && sp_env[1])
		{
			flag = ft_flag_env(sp_env[0]);
			new_node = ft_tenv_new(sp_env[0], sp_env[1], flag);
			if (!new_node)
				return (free(sp_env[0]), free(sp_env[1]), free(sp_env), NULL);
			ft_tenv_add_back(&head, new_node);
		}
		free(sp_env);
		i++;
	}
	return (head);
}
