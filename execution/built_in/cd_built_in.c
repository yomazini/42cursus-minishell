/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_built_in.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:14 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/04 15:06:47 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

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

static char	*handle_special_path(char *arg, t_data *data, int *print_flag)
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
	else if (ft_strncmp(arg, "~", 2) == 0)
		return (get_home_path(data));
	// TODO: Optional: Handle ~/path expansion here if desired
	return (arg); // Return original argument if not special
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

int	ft_cd(t_cmd *cmd, t_data *data)
{
	char	*target_path_arg;
	char	*target_path;
	int		print_path_flag;

	target_path_arg = cmd->argv[1];
	target_path = NULL;
	print_path_flag = FALSE;
	if (target_path_arg && cmd->argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		data->last_exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	if (!target_path_arg)
		target_path = get_home_path(data);
	else
		target_path = handle_special_path(target_path_arg,
				data, &print_path_flag);
	if (!target_path)
		return (EXIT_FAILURE);
	return (process_dir_change(target_path, data, print_path_flag));
}
