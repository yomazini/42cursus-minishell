/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_built_in_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 21:14:43 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/05 11:55:34 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

char	*expand_tilde_path(const char *path_arg, t_env *env_list)
{
	char	*home_dir;
	char	*expanded_path;

	if (!path_arg)
		return (NULL);
	if (path_arg[0] != '~')
		return (ft_strdup(path_arg));
	home_dir = ft_list_getenv(env_list, "HOME");
	if (!home_dir || home_dir[0] == '\0')
		return (NULL);
	if (path_arg[1] == '\0')
		return (ft_strdup(home_dir));
	if (path_arg[1] == '/')
	{
		expanded_path = ft_strjoin(home_dir, path_arg + 1);
		if (!expanded_path)
			perror("minishell: cd: malloc failed");
		return (expanded_path);
	}
	else
		return (ft_strdup(path_arg));
}

char	*ft_list_getenv(t_env *env_list, const char *name)
{
	t_env	*curr;
	size_t	name_len;

	if (!env_list || !name)
		return (NULL);
	curr = env_list;
	name_len = ft_strlen(name);
	while (curr)
	{
		if (curr->name && ft_strncmp(curr->name, name, name_len + 1) == 0)
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}
