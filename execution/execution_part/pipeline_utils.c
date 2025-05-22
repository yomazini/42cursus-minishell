/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:05:52 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/22 16:31:06 by eel-garo         ###   ########.fr       */
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

int	wait_for_pipeline(int count, pid_t last_pid, t_data *data)
{
	int		wait_status;
	pid_t	finished_pid;
	int		children_waited_for;
	int		last_status;
	bool 	flag = false;

	children_waited_for = -1;
	last_status = EXIT_SUCCESS;
	while (++children_waited_for < count)
	{
		finished_pid = waitpid(-1, &wait_status, 0);
		if (finished_pid < 0)
		{
			if (errno == ECHILD)
				break ;
			data->last_exit_status = EXIT_FAILURE;
			last_status = EXIT_FAILURE;
			break ;
		}
		 if (WIFSIGNALED(wait_status) && WTERMSIG(wait_status) == SIGINT)
            flag = true;
		if (finished_pid == last_pid)
		{
			update_last_exit_status(data, wait_status);
			last_status = data->last_exit_status;
		}
	}
	if (flag && data->last_exit_status != 130) 
        write(STDOUT_FILENO, "\n", 1);
	flag = false;
	return (last_status);
}
