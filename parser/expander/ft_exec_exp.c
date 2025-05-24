/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:42:43 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/24 15:06:18 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"
#include "../execution/exec_header.h"

static char	*ft_helper_exit_status_func(char *new_str, t_data *data, int *i)
{
	extern int	g_global;

	if (g_global == 2)
	{
		data->last_exit_status = 130;
		g_global = 0;
	}
	else if (data->echo_pipe_flag)
	{
		data->last_exit_status = 0;
		data->echo_pipe_flag = false;
	}
	new_str = ft_append_exit_status(new_str, data->last_exit_status);
	*i += 2;
	return (new_str);
}

static char	*ft_expenv(char *new_str, const char *orign, t_data *data, int *i)
{
	data->peak = ft_peakahead(orign[1]);
	if (new_str && data->peak == -1)
	{
		new_str = append_single_char(new_str, orign[0]);
		if (new_str)
			(*i)++;
	}
	else if (new_str && (data->peak == 1 || data->peak == 2))
	{
		new_str = ft_append_variable(new_str, orign, data, i);
		if (!new_str)
			return (NULL);
	}
	else if (new_str && data->peak == 3)
		new_str = ft_helper_exit_status_func(new_str, data, i);
	else if (data->peak == 4)
		*i += 2;
	else
	{
		new_str = append_single_char(new_str, orign[0]);
		if (new_str)
			(*i)++;
	}
	return (new_str);
}

static void	ft_handle_doller_expantion(t_exp_p *st, const char *org_str)
{
	const char	*proc;

	if (!*(st->qt_ptr))
		st->data->field_splitting_needed = true;
	else
		st->data->field_splitting_needed = false;
	proc = &org_str[*(st->i_ptr)];
	*(st->ptr) = ft_expenv(*(st->ptr), proc, st->data, st->i_ptr);
}

static bool	ft_process_expansion(t_exp_p *st, const char *org)
{
	char		curr_char_vl;

	curr_char_vl = org[*(st->i_ptr)];
	if (ft_isquot(curr_char_vl) && !*(st->qt_ptr))
	{
		*(st->qt_ptr) = curr_char_vl;
		(*(st->i_ptr))++;
	}
	else if (*(st->qt_ptr) && curr_char_vl == *(st->qt_ptr))
	{
		*(st->qt_ptr) = '\0';
		(*(st->i_ptr))++;
	}
	else if (curr_char_vl == '$' && org[*(st->i_ptr) + 1]
		&& st->data->herdoc == false
		&& (*(st->qt_ptr) == '\"' || !*(st->qt_ptr)))
	{
		ft_handle_doller_expantion(st, org);
	}
	else
		*(st->ptr) = append_single_char(*(st->ptr), org[(*(st->i_ptr))++]);
	return (*(st->ptr) != NULL);
}

char	*ft_build_expanded_string(const char *orign, t_data *data)
{
	int		i;
	char	quote_char;
	char	*new_str;
	t_exp_p	state;

	new_str = ft_strdup("");
	if (!new_str)
		return (NULL);
	i = 0;
	quote_char = '\0';
	state.ptr = &new_str;
	state.i_ptr = &i;
	state.qt_ptr = &quote_char;
	state.data = data;
	while (orign && orign[i])
	{
		if (orign[i] == '$' && !quote_char
			&& orign[i + 1] && ft_isquot(orign[i + 1]))
			i++;
		if (!orign[i])
			break ;
		if (!ft_process_expansion(&state, orign))
			return (NULL);
	}
	return (new_str);
}
