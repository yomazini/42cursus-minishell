/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:42:43 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/07 15:38:06 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"
#include "../execution/exec_header.h"

static char	*ft_helper_exit_status_func(char *new_str, t_data *data, int *i)
{
	extern int	g_global_signal;
	if (g_global_signal)
	{
		data->last_exit_status = 130;
		g_global_signal = 0;
	}
	new_str = ft_append_exit_status(new_str, data->last_exit_status);
	*i += 2;
	return (new_str);
	
}
char	*ft_expenv(char *new_str, const char *orign, t_data *data, int *i)
{
	int			peak;
	extern int	g_global_signal;

	peak = ft_peakahead(orign[1]);
	if (peak == -1)
	{
		new_str = append_single_char(new_str, orign[0]);
		(*i)++;
	}
	else if (peak == 1 || peak == 2)
	{
		new_str = ft_append_vt(new_str, orign, data->env_list, i, peak);
		if (!new_str)
			return (NULL);
	}
	else if (peak == 3)
		new_str = ft_helper_exit_status_func(new_str, data, i);
	else if (peak == 4)
		*i += 2;
	return (new_str);
}

char *ft_build_expanded_string(const char *orign, t_data *data)
{
	int		i;
	char	quote_char;
	char	*new_str;
	const char *process;

	new_str = ft_strdup("");
	if (!new_str)
		return (NULL);
	i = 0;
	quote_char = '\0';
	while (orign && orign[i])
	{
		if (orign[i] == '$' && !quote_char && ft_isquot(orign[i + 1]))
			i++;
		if (orign[i] && ft_isquot(orign[i]) && !quote_char)
		{
			quote_char = orign[i];
			new_str = append_single_char(new_str, orign[i++]);
		}
		else if (orign[i] && quote_char && orign[i] == quote_char)
		{
			quote_char = '\0';
			new_str = append_single_char(new_str, orign[i++]);
		}
		else if (orign[i] == '$' && orign[i + 1] && data->herdoc == false && (quote_char == '\"' || !quote_char))
		{
			process = &orign[i];
			new_str = ft_expenv(new_str, process, data, &i);
		}
		else if (orign[i])
			new_str = append_single_char(new_str, orign[i++]);
	}
	return (new_str);
}

void	ft_expand(t_token **token, t_data *data)
{
	char	*orig_value;
	char	*exp_value;

	if (!token|| !*token|| !(*token)->value)
		return ;
	orig_value = (*token)->value;
	exp_value = ft_build_expanded_string(orig_value, data);
	if (!exp_value)
	{
		exp_value = ft_strdup("");
		if (!exp_value)
		{
			free(orig_value);
			(*token)->value = NULL;
			return ;
		}
	}
	if (data->herdoc == true)
		data->herdoc = false;
	free(orig_value);
	(*token)->value = exp_value;	
}

