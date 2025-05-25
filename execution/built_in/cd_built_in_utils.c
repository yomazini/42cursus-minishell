/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_built_in_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:21:27 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/25 16:13:55 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	process_dir_change(char *target, t_data *data, int print_path_flag)
{
	char	*current_pwd;
	int		chdir_ret;

	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		perror("minishell: cd: error retrieving current directory");
		data->last_exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	chdir_ret = chdir(target);
	if (chdir_ret == -1)
		return (cd_error(target, data, current_pwd));
	if (print_path_flag)
		ft_putendl_fd(target, STDOUT_FILENO);
	if (update_pwd_env_vars(data, current_pwd) == -1)
	{
		(free(current_pwd), current_pwd = NULL);
		data->last_exit_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	(free(current_pwd), current_pwd = NULL);
	data->last_exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}

int	cd_error(char *path, t_data *data, char *old_pwd_to_free)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	(free(old_pwd_to_free), old_pwd_to_free = NULL);
	data->last_exit_status = EXIT_FAILURE;
	return (EXIT_FAILURE);
}

static int	update_existing_env_node(t_env *node, const char *value)
{
	free(node->value);
	node->value = NULL;
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
			return (-1);
	}
	return (0);
}

static int	add_new_env_node(t_env **list_head, t_env *last_node,
							const char *name, const char *value)
{
	char	*new_name;
	char	*new_value;
	t_env	*new_node;

	new_name = ft_strdup(name);
	if (!new_name)
		return (-1);
	new_value = NULL;
	if (value)
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return (free(new_name), -1);
	}
	new_node = ft_tenv_new(new_name, new_value, 0);
	if (!new_node)
		return (free(new_name), free(new_value), -1);
	if (last_node)
		last_node->next = new_node;
	else
		*list_head = new_node;
	return (0);
}

int	ft_list_setenv(t_env **env_list_head, const char *name, const char *value)
{
	t_env	*curr;
	t_env	*last;
	size_t	name_len;

	curr = *env_list_head;
	last = NULL;
	name_len = ft_strlen(name);
	while (curr)
	{
		if (curr->name && ft_strncmp(curr->name, name, name_len + 1) == 0)
			return (update_existing_env_node(curr, value));
		last = curr;
		curr = curr->next;
	}
	return (add_new_env_node(env_list_head, last, name, value));
}
