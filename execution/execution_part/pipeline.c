/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-garo <eel-garo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:01:41 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/23 17:30:58 by eel-garo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

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

static	pid_t	fork_and_exec_child(t_cmd *cmd, t_data *data, int prev_read_end,
										int pipe_fd[2])
{
	pid_t		pid;
	extern int	g_global;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	else if (pid == 0)
	{
		setup_child_pipes(prev_read_end, pipe_fd, cmd);
		execute_command_node(cmd, data);
	}
	return (pid);
}

static int	handle_pipe_fork_errors(t_cmd *cmd, int prev_fd, int pipe_fd[2])
{
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	perror("minishell: fork");
	return (EXIT_FAILURE);
}

static int	pipeline_loop(t_cmd *cmd_list, t_data *data, t_pipeline_ctx *ctx)
{
	pid_t	pid;

	while (cmd_list)
	{
		if (cmd_list->next && pipe(ctx->pipe_fd) == -1)
		{
			if (ctx->prev_read != STDIN_FILENO)
				close(ctx->prev_read);
			perror("pipe");
			return (EXIT_FAILURE);
		}
		pid = fork_and_exec_child(cmd_list, data, ctx->prev_read, ctx->pipe_fd);
		if (pid < 0)
			return (handle_pipe_fork_errors(cmd_list,
					ctx->prev_read, ctx->pipe_fd));
		ctx->last_pid = pid;
		parent_pipe_handler(&ctx->prev_read, ctx->pipe_fd, cmd_list);
		cmd_list = cmd_list->next;
	}
	return (EXIT_SUCCESS);
}

int	execute_pipeline(t_cmd *cmd_list, t_data *data)
{
	t_pipeline_ctx	ctx;
	int				cmd_count;
	int				rc;

	ctx.prev_read = STDIN_FILENO;
	ctx.last_pid = -1;
	ctx.pipe_fd[0] = -1;
	ctx.pipe_fd[1] = -1;
	cmd_count = count_commands(cmd_list, data);
	if (cmd_count == 0)
		return (EXIT_SUCCESS);
	rc = pipeline_loop(cmd_list, data, &ctx);
	if (ctx.prev_read != STDIN_FILENO)
		close(ctx.prev_read);
	if (rc != EXIT_SUCCESS)
		return (rc);
	return (wait_for_pipeline(cmd_count, ctx.last_pid, data));
}
