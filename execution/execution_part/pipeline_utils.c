/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:05:52 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/23 23:04:14 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

void	setup_child_pipes(int prev_pipe_read, int pipe_fd[2], t_cmd *cmd)
{
	if (prev_pipe_read != STDIN_FILENO)
	{
		if (dup2(prev_pipe_read, STDIN_FILENO) < 0)
			exit(EXIT_FAILURE);
		close(prev_pipe_read);
	}
	if (cmd->next != NULL)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
			exit(EXIT_FAILURE);
		close(pipe_fd[1]);
	}
}

static	void	ft_assign_vars_2_wait(int *flag,
		int *childs_wait_for, int *last_status)
{
	*flag = false;
	*childs_wait_for = -1;
	*last_status = EXIT_SUCCESS;
}

static int	handle_wait_errno(t_data *data)
{
	if (errno == ECHILD)
		return (0);
	perror("minishell: waitpid error");
	data->last_exit_status = EXIT_FAILURE;
	return (EXIT_FAILURE);
}

static void	prnit_nwln(int flag, t_data *data)
{
	if (flag && data->last_exit_status != 130)
		write(STDOUT_FILENO, "\n", 1);
}

int	wait_for_pipeline(int count, pid_t last_pid, t_data *data)
{
	int		wait_status;
	pid_t	finished_pid;
	int		childs_wait_for;
	int		last_status;
	int		flag;

	ft_assign_vars_2_wait(&flag, & childs_wait_for, &last_status);
	while (++childs_wait_for < count)
	{
		finished_pid = waitpid(-1, &wait_status, 0);
		if (finished_pid < 0)
		{
			if (handle_wait_errno(data) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			return (last_status);
		}
		if (WIFSIGNALED(wait_status) && WTERMSIG(wait_status) == SIGINT)
			flag = true;
		if (finished_pid == last_pid)
		{
			update_last_exit_status(data, wait_status);
			last_status = data->last_exit_status;
		}
	}
	return (prnit_nwln(flag, data), flag = false, last_status);
}
