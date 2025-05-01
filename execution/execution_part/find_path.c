/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazinistudent.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:41:51 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/22 16:49:18 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

 int	is_executable(const char *path)
{
	struct stat statbuf;
	if (!path)
		return 0;

	if ((stat(path, &statbuf) == -1))
		return 0;
	if (access(path, X_OK) == 0)
		return 1;
	return 0;
}

 char	*search_path_variable(const char *command_name, t_env *env_list)
{
	char	*path_var_value;
	char	**path_dirs;
	char	*temp_path;
	char	*full_path;
	int		i;

	path_var_value = ft_list_getenv(env_list, "PATH");
	if (!path_var_value)
		return (NULL);
	path_dirs = ft_split(path_var_value, ':');
	if (!path_dirs)
		return (NULL);
	i = 0;
	while (path_dirs[i])
	{
		temp_path = ft_strjoin(path_dirs[i], "/");
		if (!temp_path)
			return (free_arr(path_dirs) ,NULL);
	
		full_path = ft_strjoin(temp_path, command_name);
		free(temp_path);
		if (!full_path)
			return (free_arr(path_dirs) , NULL);
		if (is_executable(full_path))
			return (free_arr(path_dirs) , full_path);
		free(full_path); 
		i++;
	}
	return (free_arr(path_dirs), NULL);
}

char	*find_command_path(const char *command_name, t_env *env_list)
{
	if (!command_name || command_name[0] == '\0')
				return (NULL);

	if (ft_strchr(command_name, '/') != NULL)
	{
		if (is_executable(command_name))
			return (ft_strdup(command_name));
		else
			return (NULL);
	}
	else
		return (search_path_variable(command_name, env_list));
}
