/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:38:35 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/20 18:49:06 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	count_commands(t_cmd *cmd_list, t_data *data)
{
	int		count;
	t_cmd	*counter;

	count = 0;
	counter = cmd_list;
	while (counter)
	{
		count++;
		counter = counter->next;
	}
	if (count > 1)
	{
		data->print_flag = FALSE;
	}
	return (count);
}
