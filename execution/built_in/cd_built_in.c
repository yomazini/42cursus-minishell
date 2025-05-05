/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_built_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:14 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/05 12:08:09 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static char	*get_home_path(t_data *data)
{
	char	*target_path;

	target_path = ft_list_getenv(data->env_list, "HOME");
	if (!target_path || target_path[0] == '\0')
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		data->last_exit_status = EXIT_FAILURE;
		return (NULL);
	}
	return (target_path);
}

static char	*handle_dash_path(char *arg, t_data *data, int *print_flag)
{
	char	*target_path;

	*print_flag = FALSE;
	if (ft_strncmp(arg, "-", 2) == 0)
	{
		target_path = ft_list_getenv(data->env_list, "OLDPWD");
		if (!target_path || target_path[0] == '\0')
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			data->last_exit_status = EXIT_FAILURE;
			return (NULL);
		}
		*print_flag = TRUE;
		return (target_path);
	}
	return (arg);
}

int	update_pwd_env_vars(t_data *data, char *old_pwd_path)
{
	char	*new_pwd_path;
	int		ret1;
	int		ret2;

	ret1 = ft_list_setenv(&data->env_list, "OLDPWD", old_pwd_path);
	new_pwd_path = getcwd(NULL, 0);
	if (!new_pwd_path)
	{
		perror("minishell: cd: error retrieving new directory path");
		ft_list_setenv(&data->env_list, "PWD", "");
		return (-1);
	}
	ret2 = ft_list_setenv(&data->env_list, "PWD", new_pwd_path);
	free(new_pwd_path);
	if (ret1 == -1 || ret2 == -1)
	{
		ft_putstr_fd("minishell: cd: error update env variables\n", 2);
		return (-1);
	}
	return (0);
}

static char	*determine_cd_target(t_cmd *cmd, t_data *data,
								int *print_path_flag, char **path_to_free)
{
	char	*path_after_dash;

	*path_to_free = NULL;
	*print_path_flag = FALSE;
	if (cmd->argv[1])
	{
		path_after_dash = handle_dash_path(cmd->argv[1],
				data, print_path_flag);
		if (!path_after_dash)
			return (NULL);
		if (*print_path_flag == TRUE)
			return (path_after_dash);
		else
		{
			*path_to_free = expand_tilde_path(path_after_dash, data->env_list);
			if (!*path_to_free)
			{
				if (!ft_list_getenv(data->env_list, "HOME"))
					ft_putstr_fd("minishell: cd: HOME not set\n", 2);
				data->last_exit_status = EXIT_FAILURE;
				return (NULL);
			}
		}
	}
	return (*path_to_free);
}

int	ft_cd(t_cmd *cmd, t_data *data)
{
	char	*final_target_path;
	char	*allocated_path;
	int		print_path_flag;
	int		result;

	allocated_path = NULL;
	if (!cmd->argv[1])
		final_target_path = get_home_path(data);
	else
		final_target_path = determine_cd_target(cmd, data,
				&print_path_flag, &allocated_path);
	if (!final_target_path)
		return (EXIT_FAILURE);
	result = process_dir_change(final_target_path, data, print_path_flag);
	free(allocated_path);
	return (result);
}
