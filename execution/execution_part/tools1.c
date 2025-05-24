/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:08:59 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/23 20:57:40 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	ft_is_only_whitespace(char *arg)
{
	int	i;

	i = 0;
	if (!arg || arg[0] == '\0')
		return (0);
	while (arg[i] == ' ' || (arg[i] >= '\t' && '\r' >= arg[i]))
		i++;
	return (arg[i] == '\0');
}

static void	ft_assign_2_vld_nmr(int *i, int *dg_co, size_t *len, char **s)
{
	char	*trimmed_char;

	*i = 0;
	*dg_co = 0;
	trimmed_char = ft_strtrim(*s, " ");
	*s = trimmed_char;
	*len = ft_strlen(*s);
}

int	ft_valid_number(char *s)
{
	int		i;
	size_t	len;
	int		digit_count;

	ft_assign_2_vld_nmr(&i, &digit_count, &len, &s);
	if (!s || !s[0])
		return (free(s), 0);
	if ((s[i] == '-' || s[i] == '+') && i++)
	{
		if (!s[i])
			return (free(s), 0);
	}
	if (!ft_isdigit(s[i]))
		return (free(s), 0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (free(s), 0);
		(1 && digit_count++ && i++);
	}
	if (digit_count == 0 || (digit_count > 19
			&& (len > 20 || (len == 20 && s[0] != '-')))
		|| (digit_count > 19 && len > 19 && s[0] == '-'))
		return (free(s), 0);
	return (free(s), 1);
}

int	is_parent_builtin(t_cmd *cmd)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (FALSE);
	name = cmd->argv[0];
	if ((ft_strncmp(name, "pwd", 4) == 0)
		|| (ft_strncmp(name, "echo", 5) == 0)
		|| (ft_strncmp(name, "env", 4) == 0)
		|| (ft_strncmp(name, "exit", 5) == 0)
		|| (ft_strncmp(name, "cd", 3) == 0)
		|| (ft_strncmp(name, "unset", 6) == 0)
		|| (ft_strncmp(name, "export", 7) == 0))
		return (TRUE);
	return (FALSE);
}
