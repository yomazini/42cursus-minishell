/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:01:41 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/17 20:41:17 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static int	count_commands(t_cmd *cmd_list, t_data *data)
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

static void	parent_pipe_handler(int *prev_pipe_read_end, int pipe_fd[2],
								t_cmd *current_cmd)
{
	if (*prev_pipe_read_end != STDIN_FILENO)
		close(*prev_pipe_read_end);
	if (current_cmd->next != NULL)
	{
		close(pipe_fd[1]);
		*prev_pipe_read_end = pipe_fd[0];
	}
}

static	void	initialize_pipeline_vars(int *prev_pipe_read_end
									, pid_t *last_pid, int *pipe0, int *pipe1)
{
	*prev_pipe_read_end = STDIN_FILENO;
	*last_pid = -1;
	*pipe0 = -1;
	*pipe1 = -1;
}

static	pid_t	fork_and_exec_child(t_cmd *cmd, t_data *data, int prev_read_end,
										int pipe_fd[2])
{
	pid_t		pid;
	extern int	g_tmp;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	else if (pid == 0)
	{
		setup_child_pipes(prev_read_end, pipe_fd, cmd);
		execute_command_node(cmd, data);
	}
	g_tmp = 1;
	set_signal_handlers_prompt();
	return (pid);
}

int	execute_pipeline(t_cmd *cmd_list, t_data *data)
{
	int		pipe_fd[2];
	int		prev_pipe_read_end;
	pid_t	pid;
	pid_t	last_pid;
	int		command_count;

	initialize_pipeline_vars(&prev_pipe_read_end,
		&last_pid, &pipe_fd[0], &pipe_fd[1]);
	command_count = count_commands(cmd_list, data);
	if (command_count == 0)
		return (EXIT_SUCCESS);
	while (cmd_list != NULL)
	{
		if (cmd_list->next && pipe(pipe_fd) == -1)
		{
			if (prev_pipe_read_end != STDIN_FILENO) 
				close(prev_pipe_read_end);
			return (perror("pipe"), EXIT_FAILURE);
		}
		pid = fork_and_exec_child(cmd_list, data, prev_pipe_read_end, pipe_fd);
		if (pid < 0)
		{
    		if (prev_pipe_read_end != STDIN_FILENO)
				close(prev_pipe_read_end);
    		if (cmd_list->next)
				(close(pipe_fd[0]), close(pipe_fd[1]));
    		return (perror("minishell: fork") ,EXIT_FAILURE);
		}
		last_pid = pid;
		parent_pipe_handler(&prev_pipe_read_end, pipe_fd, cmd_list);
		cmd_list = cmd_list->next;
	}
	if (prev_pipe_read_end != STDIN_FILENO)
		close(prev_pipe_read_end);
	return (wait_for_pipeline(command_count, last_pid, data));
}
