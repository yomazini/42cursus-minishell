/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:01:41 by ymazini           #+#    #+#             */
/*   Updated: 2025/04/29 18:32:16 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static void	execute_command_node(t_cmd *cmd, t_data *data)
{
	int		builtin_status;
	char	*path;
	char	**envp_array;
	if (apply_redirections(cmd) != 0)
		exit(EXIT_FAILURE);

	builtin_status = execute_built_ins(cmd, data);

	if (builtin_status != -1)
		exit(builtin_status);
	else
	{
		envp_array = convert_envlist_to_array(data->env_list);
		if (!envp_array) exit(EXIT_FAILURE);
		path = find_command_path(cmd->argv[0], data->env_list);
		if (!path)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			free_arr(envp_array);
			exit(127);
		}
		execve(path, cmd->argv, envp_array);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd->argv[0]);
		free(path);
		free_arr(envp_array);
		exit(errno == EACCES ? 126 : EXIT_FAILURE);
	}
}

int	execute_pipeline(t_cmd *cmd_list, t_data *data)
{
	int		pipe_fd[2];
	int		prev_pipe_read_end;
	pid_t	pid;
	t_cmd	*current_cmd;
	int		last_cmd_status_interpreted;
	pid_t	last_pid;
	int		command_count;
	int		children_waited_for;

	prev_pipe_read_end = STDIN_FILENO;
	current_cmd = cmd_list;
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	last_pid = -1;
	last_cmd_status_interpreted = EXIT_SUCCESS;
	command_count = 0;
	children_waited_for = 0;

	t_cmd *counter = cmd_list;
	while (counter) {
		command_count++;
		counter = counter->next;
	}
	if (command_count == 0) return (EXIT_SUCCESS);
	while (current_cmd != NULL)
	{
		if (current_cmd->next != NULL) {
			if (pipe(pipe_fd) == -1) { return (EXIT_FAILURE); }
		}

		pid = fork();
		if (pid < 0) {return (EXIT_FAILURE); }

		if (pid == 0) {
			if (prev_pipe_read_end != STDIN_FILENO) {
				if (dup2(prev_pipe_read_end, STDIN_FILENO) < 0) exit(1);
				close(prev_pipe_read_end);
			}
			if (current_cmd->next != NULL) {
				close(pipe_fd[0]);
				if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) exit(1);
				close(pipe_fd[1]);
			}
			execute_command_node(current_cmd, data);
		}
		else {
			last_pid = pid;
			if (prev_pipe_read_end != STDIN_FILENO) close(prev_pipe_read_end);
			if (current_cmd->next != NULL) {
				close(pipe_fd[1]);
				prev_pipe_read_end = pipe_fd[0];
			}
			current_cmd = current_cmd->next;
		}
	}
	if (prev_pipe_read_end != STDIN_FILENO) {
        close(prev_pipe_read_end);
    }

	int	wait_status;
	pid_t finished_pid;

	while (children_waited_for < command_count)
	{
		finished_pid = waitpid(-1, &wait_status, 0);

		if (finished_pid < 0)
		{
			if (errno == ECHILD) {
				// No more children exist (this shouldn't happen if count is correct)
				break;
			}
			else {
				perror("minishell: waitpid error");
                last_cmd_status_interpreted = EXIT_FAILURE;
				break;
			}
		}
		children_waited_for++;
		if (finished_pid == last_pid)
		{
			update_last_exit_status(data, wait_status);
			last_cmd_status_interpreted = data->last_exit_status;
		}
	}
	return (last_cmd_status_interpreted);
}
	