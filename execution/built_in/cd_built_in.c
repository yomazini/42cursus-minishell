/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_built_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:14 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/21 20:57:49 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

char	*ft_list_getenv(t_env *env_list, const char *name) 
{
    t_env *curr = env_list;
    size_t name_len = ft_strlen(name);
    while (curr) 
	{
        if (curr->name && ft_strncmp(curr->name, name, name_len + 1) == 0) 
            return curr->value;
        curr = curr->next;
    }
    return NULL;
}


int ft_list_setenv(t_env **env_list_head, const char *name, const char *value) 
{
    t_env *curr = *env_list_head;
    t_env *last = NULL;
    char *new_name = NULL;
    char *new_value = NULL;
    size_t name_len = ft_strlen(name);

    while (curr) 
	{
        if (curr->name && ft_strncmp(curr->name, name, name_len + 1) == 0) 
		{
            free(curr->value);
            curr->value = NULL;
            if (value)
			{
                curr->value = ft_strdup(value);
                if (!curr->value) 
					return (-1);
            }
            return (0);
        }
        last = curr;
        curr = curr->next;
    }
    new_name = ft_strdup(name);
    if (!new_name) 
		return (-1);
    if (value) 
	{
        new_value = ft_strdup(value);
        if (!new_value)
			return (free(new_name), -1);
    } 
	else
        new_value = NULL;
    t_env *new_node = ft_tenv_new(new_name, new_value, 0);
    if (!new_node) 
		return (free(new_name), free(new_value), -1);
    if (last)
        last->next = new_node;
    else
        *env_list_head = new_node;
    return (0);
}


static int cd_error(char *path, t_data *data, char *old_pwd_to_free)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	free(old_pwd_to_free);
	data->last_exit_status = EXIT_FAILURE;
	return (EXIT_FAILURE);
}

int ft_cd(t_cmd *cmd, t_data *data)
{
	char	*target_path_arg;
	char	*target_path;
	char	*current_pwd;
	char	*new_pwd;
	int		chdir_ret;
	int		setenv_ret1;
	int		setenv_ret2;
	
	target_path_arg = cmd->argv[1];
	target_path = NULL;
	if (cmd->argv[2])
	{
		ft_putstr_fd("cd: string not in pwd: " ,STDERR_FILENO);
		ft_putstr_fd(cmd->argv[1] ,STDERR_FILENO);
		ft_putchar_fd('\n',STDERR_FILENO);
		data->last_exit_status = EXIT_FAILURE;
		return(data->last_exit_status);
	}
	if (!target_path_arg)
	{
		target_path = ft_list_getenv(data->env_list, "HOME");
		if (!target_path || target_path[0] == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			data->last_exit_status = EXIT_FAILURE;
			return (EXIT_FAILURE);
		}
	}
	else
	{
		if (ft_strncmp(target_path_arg, "-", 2) == 0)
		{
			target_path = ft_list_getenv(data->env_list, "OLDPWD");
			if (!target_path || target_path[0] == '\0')
			{
				ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
				data->last_exit_status = EXIT_FAILURE;
				return (EXIT_FAILURE);
			}
			ft_putendl_fd(target_path, STDOUT_FILENO);
		}
		else if (ft_strncmp(target_path_arg, "~", 2) == 0)
		{
			target_path = ft_list_getenv(data->env_list, "HOME");
            if (!target_path || target_path[0] == '\0')
            {
                ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
                data->last_exit_status = EXIT_FAILURE;
                return (EXIT_FAILURE);
            }
		}
		else
			target_path = target_path_arg;
	}
	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		perror("minishell: cd: error retrieving current directory");
		data->last_exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	chdir_ret = chdir(target_path);
	if (chdir_ret == -1)
		return (cd_error(target_path, data, current_pwd));
	setenv_ret1 = ft_list_setenv(&data->env_list, "OLDPWD", current_pwd);
	free(current_pwd);
	current_pwd = NULL;
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("minishell: cd: error retrieving new directory path");
		data->last_exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	setenv_ret2 = ft_list_setenv(&data->env_list, "PWD", new_pwd);
	free(new_pwd);
	if (setenv_ret1 == -1 || setenv_ret2 == -1)
	{
		ft_putstr_fd("minishell: cd: error updating environment variables\n", STDERR_FILENO);
		data->last_exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	data->last_exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
