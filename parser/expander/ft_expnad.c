/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expnad.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:35:14 by eel-garo          #+#    #+#             */
/*   Updated: 2025/05/25 13:58:47 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parser.h"

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
	free(orig_value);
	(*token)->value = exp_value;
}
