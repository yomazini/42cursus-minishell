/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_built_in.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:06:04 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/22 14:47:07 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int ft_list_unsetenv(t_env **env_list_head, const char *name)
{
    t_env *current;
    t_env *prev;
    size_t name_len;

    if (!env_list_head || !*env_list_head || !name)
        return (0);
    current = *env_list_head;
    prev = NULL;
    name_len = ft_strlen(name);

    while (current != NULL)
    {
        if (current->name && ft_strncmp(current->name, name, name_len + 1) == 0)
        {
            if (prev == NULL)
                *env_list_head = current->next;
            else
                prev->next = current->next;
            free(current->name);
            free(current->value);
            free(current);
            current = NULL;
            return (0);
        }
        prev = current;
        current = current->next;
    }
    return (0);
}

int	ft_unset(t_cmd *cmd, t_data *data)
{
	int		i;
	int		return_status;
	char	*var_name;

	return_status = EXIT_SUCCESS;
	i = 1;
	if (cmd->argv[i] == NULL)
	{
		data->last_exit_status = EXIT_SUCCESS;
		return (EXIT_SUCCESS);
	}
	while (cmd->argv[i] != NULL)
	{
		var_name = cmd->argv[i];
		if (!ft_is_valid_identifier(var_name))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(var_name, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return_status = EXIT_FAILURE;
		}
		else
			ft_list_unsetenv(&data->env_list, var_name);
		i++;
	}
	data->last_exit_status = return_status;
	return (return_status);
}
