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

// int	is_executable(const char *path)
// {
// 	struct stat	statbuf;

// 	if (!path)
// 		return (0);
// 	if ((stat(path, &statbuf) == -1))
// 		return (0);
// 	if (access(path, X_OK) == 0)
// 		return (1);
// 	return (0);
// }

int	is_executable(const char *path)
{
	struct stat	statbuf;

	if (!path)
	{
		errno = EINVAL; // Invalid argument
		return (0);
	}
	// 1. Check existence and get file type using stat()
	if (stat(path, &statbuf) == -1)
	{
		// errno is set by stat() (e.g., ENOENT, EACCES on a path component)
		return (0);
	}

	// 2. Check if it's a directory
	if (S_ISDIR(statbuf.st_mode))
	{
		errno = EISDIR; // Set errno to "Is a directory"
		return (0);     // Cannot execute directories as commands
	}

	// 3. Check if it's a regular file and has execute permission using access()
	if (S_ISREG(statbuf.st_mode) && access(path, X_OK) == 0)
	{
		return (1); // It's an executable regular file
	}
	else
	{
		// If access failed, errno is set by access() (likely EACCES).
		// If not a regular file (e.g. symlink to non-exec, device),
		// access(X_OK) might also fail or succeed depending on target.
		// Ensure EACCES if it's not already a specific permission error.
		if (errno != EACCES && errno != EPERM)
			errno = EACCES; // Default to permission error
		return (0);
	}
}

static char	**get_and_split_path(t_env *env_list)
{
	char	*path_var_value;
	char	**path_dirs;

	path_var_value = ft_list_getenv(env_list, "PATH");
	if (!path_var_value || path_var_value[0] == '\0')
	{
		return (NULL);
	}
	path_dirs = ft_split(path_var_value, ':');
	if (!path_dirs)
	{
		perror("minishell: ft_split failed for PATH");
		return (NULL);
	}
	return (path_dirs);
}

static char	*check_path_entry(const char *dir, const char *command_name)
{
	char	*temp_slash;
	char	*full_path;

	temp_slash = ft_strjoin(dir, "/");
	if (!temp_slash)
	{
		perror("minishell: ft_strjoin failed");
		return (NULL);
	}
	full_path = ft_strjoin(temp_slash, command_name);
	free(temp_slash);
	if (!full_path)
	{
		perror("minishell: ft_strjoin failed");
		return (NULL);
	}
	if (is_executable(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*search_path_variable(const char *command_name, t_env *env_list)
{
	char	**path_dirs;
	char	*found_path;
	int		i;

	path_dirs = get_and_split_path(env_list);
	if (!path_dirs)
		return (NULL);
	i = 0;
	found_path = NULL;
	while (path_dirs[i] != NULL)
	{
		found_path = check_path_entry(path_dirs[i], command_name);
		if (found_path)
			break ;
		i++;
	}
	free_arr(path_dirs);
	return (found_path);
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
