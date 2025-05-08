/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:42:43 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/08 11:04:13 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"
#include "../execution/exec_header.h"

static char	*ft_helper_exit_status_func(char *new_str, t_data *data, int *i)
{
	// extern int	g_global_signal;

	// if (g_global_signal)
	// {
	// 	data->last_exit_status = 130;
	// 	g_global_signal = 0;
	// }
	new_str = ft_append_exit_status(new_str, data->last_exit_status);
	*i += 2;
	return (new_str);
}

char	*ft_expenv(char *new_str, const char *orign, t_data *data, int *i)
{
	extern int	g_global_signal;

	data->peak = ft_peakahead(orign[1]);
	if (data->peak == -1)
	{
		new_str = append_single_char(new_str, orign[0]);
		(*i)++;
	}
	else if (data->peak == 1 || data->peak == 2)
	{
		new_str = ft_append_vt(new_str, orign, data, i);
		if (!new_str)
			return (NULL);
	}
	else if (data->peak == 3)
		new_str = ft_helper_exit_status_func(new_str, data, i);
	else if (data->peak == 4)
		*i += 2;
	return (new_str);
}

static bool	ft_process_expansion(t_exp_p *st, const char *org)
{
	const char	*proc;
	char		curr_char_vl;

	curr_char_vl = org[*(st->i_ptr)];
	if (ft_isquot(curr_char_vl) && !*(st->qt_ptr))
	{
		*(st->qt_ptr) = curr_char_vl;
		*(st->ptr) = append_single_char(*(st->ptr), org[(*(st->i_ptr))++]);
	}
	else if (*(st->qt_ptr) && curr_char_vl == *(st->qt_ptr))
	{
		*(st->qt_ptr) = '\0';
		*(st->ptr) = append_single_char(*(st->ptr), org[(*(st->i_ptr))++]);
	}
	else if (curr_char_vl == '$' && org[*(st->i_ptr) + 1]
		&& st->data->herdoc == false
		&& (*(st->qt_ptr) == '\"' || !*(st->qt_ptr)))
	{
		proc = &org[*(st->i_ptr)];
		*(st->ptr) = ft_expenv(*(st->ptr), proc, st->data, st->i_ptr);
	}
	else
		*(st->ptr) = append_single_char(*(st->ptr),
				org[(*(st->i_ptr))++]);
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

void	ft_expand(t_token **token, t_data *data)
{
	char	*orig_value;
	char	*exp_value;

	if (!token || !*token || !(*token)->value)
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
