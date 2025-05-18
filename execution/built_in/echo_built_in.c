/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazini <ymazini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:46:19 by ymazini           #+#    #+#             */
/*   Updated: 2025/05/18 17:20:09 by ymazini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec_header.h"

static	int	ft_check_n_flag(char *s)
{
	int	i;

	i = 0;
	if (s && s[0] == '-' && s[1] == 'n')
	{
		i = 2;
		while (s[i] == 'n')
			i++;
		return (s[i] == '\0');
	}
	return (0);
}

int	get_printing_start_index(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
	{
		if (ft_check_n_flag(cmd->argv[i]))
			i++;
		else
			break ;
	}
	return (i);
}

void	prepare_echo(t_cmd *cmd, int *print_newline_flag, int *start_index)
{
	int	i;

	i = 1;
	*print_newline_flag = TRUE;
	while (cmd->argv[i])
	{
		if (ft_check_n_flag(cmd->argv[i])
			|| ft_is_only_whitespace(cmd->argv[i]))
		{
			if (ft_check_n_flag(cmd->argv[i]))
				*print_newline_flag = FALSE;
			i++;
		}
		else
			break ;
	}
	*start_index = get_printing_start_index(cmd);
}

int	ft_echo(t_cmd *cmd, t_data *data)
{
	int	flag_print_newline;
	int	start_index;
	int	i;

	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		data->last_exit_status = EXIT_SUCCESS;
		return (EXIT_SUCCESS);
	}
	prepare_echo(cmd, &flag_print_newline, &start_index);
	i = start_index;
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		if (cmd->argv[i + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (flag_print_newline == TRUE)
		ft_putchar_fd('\n', STDOUT_FILENO);
	data->last_exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}



// int	ft_echo(t_cmd *cmd, t_data *data)
// {
// 	int		flag_print_newline;
// 	int		start_index;
// 	int		i;
// 	char	*output_buffer; // Buffer to build the string
// 	char	*temp_join;
// 	char	*space_str;

// 	// Handle NULL cmd or empty argv (echo with no args)
// 	if (!cmd || !cmd->argv || !cmd->argv[0] || !cmd->argv[1]) // argv[0] is "echo"
// 	{
// 		ft_putchar_fd('\n', STDOUT_FILENO); // Just print newline
// 		data->last_exit_status = EXIT_SUCCESS;
// 		return (EXIT_SUCCESS);
// 	}

// 	prepare_echo(cmd, &flag_print_newline, &start_index); // Assumes this is correct

// 	output_buffer = ft_strdup(""); // Start with an empty string
// 	if (!output_buffer) {
// 		perror("minishell: echo: malloc failed");
// 		data->last_exit_status = EXIT_FAILURE;
// 		return (EXIT_FAILURE);
// 	}

// 	i = start_index;
// 	while (cmd->argv[i])
// 	{
// 		temp_join = ft_strjoin(output_buffer, cmd->argv[i]);
// 		free(output_buffer);
// 		if (!temp_join) { perror("minishell: echo: malloc failed"); data->last_exit_status = EXIT_FAILURE; return (EXIT_FAILURE); }
// 		output_buffer = temp_join;

// 		if (cmd->argv[i + 1] != NULL) // If there's a next argument, add a space
// 		{
// 			space_str = " ";
// 			temp_join = ft_strjoin(output_buffer, space_str);
// 			free(output_buffer);
// 			if (!temp_join) { perror("minishell: echo: malloc failed"); data->last_exit_status = EXIT_FAILURE; return (EXIT_FAILURE); }
// 			output_buffer = temp_join;
// 		}
// 		i++;
// 	}

// 	// Add newline to buffer if needed
// 	if (flag_print_newline == TRUE)
// 	{
// 		temp_join = ft_strjoin(output_buffer, "\n");
// 		free(output_buffer);
// 		if (!temp_join) { perror("minishell: echo: malloc failed"); data->last_exit_status = EXIT_FAILURE; return (EXIT_FAILURE); }
// 		output_buffer = temp_join;
// 	}

// 	// Write the entire buffer in one go (or ft_putstr_fd if it's optimized)
// 	if (ft_strlen(output_buffer) > 0) // Only write if there's something to write
// 	{
// 		if (write(STDOUT_FILENO, output_buffer, ft_strlen(output_buffer)) < 0)
// 		{
// 			perror("minishell: echo: write error");
// 			free(output_buffer);
// 			data->last_exit_status = EXIT_FAILURE;
// 			return (EXIT_FAILURE);
// 		}
// 	}

// 	free(output_buffer);
// 	data->last_exit_status = EXIT_SUCCESS;
// 	return (EXIT_SUCCESS);
// }
