/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:09:05 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/24 22:21:02 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

int	save_original_fds(int *original_stdin, int *original_stdout) 
{
	*original_stdin = dup(STDIN_FILENO);
	if (*original_stdin < 0)
	{
		perror("minishell: dup failed for stdin");
		return (-1);
	}
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdout < 0)
	{
		perror("minishell: dup failed for stdout");
		close(*original_stdin);
		*original_stdin = -1;
		return (-1);
	}
	return (0);
}

int	restore_original_fds(int original_stdin, int original_stdout) 
{
	int	ret_stdin = 0;
	int	ret_stdout = 0;
	if (original_stdin >= 0)
	{
		ret_stdin = dup2(original_stdin, STDIN_FILENO);
		close(original_stdin); // Close the saved fd
	}
	if (original_stdout >= 0)
	{
		ret_stdout = dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}

	if (ret_stdin < 0 || ret_stdout < 0)
	{
		perror("minishell: dup2 failed during fd restore");
		return (-1);
	}
	return (0);
}
