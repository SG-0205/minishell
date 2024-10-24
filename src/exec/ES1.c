/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ES1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:49:57 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/24 09:28:35 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_bool	is_local(t_cmd *cmd, t_cmd *start)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (FALSE);
	if ((ft_strcmp(cmd->args[0], "unset") == 0 && cmd_list_size(&start) == 1)
		|| (ft_strcmp(cmd->args[0], "export") == 0 && cmd->args[1] && !cmd->next)
		|| (ft_strcmp(cmd->args[0], "exit") == 0
			&& cmd_list_size(&start) == 1)
		|| (ft_strcmp(cmd->args[0], "cd") == 0 && cmd_list_size(&start) == 1))
		return (TRUE);
	return (FALSE);
}

t_bool	local_exec(t_cmd *cmd, t_cmd *start)
{
	int	exit_code;

	if (!cmd || cmd->is_builtin == FALSE || is_local(cmd, start) == FALSE)
		return (FALSE);
	if (redirect(cmd) != 0)
		return (-1);
	exit_code = exec_builtins(cmd->args[0], &cmd->args[1], cmd->link);
	exit_code = normalize_return(exit_code);
	update_var(cmd->link, "?", gc_itoa(exit_code, cmd->link->gc, 1));
	return (TRUE);
}

void	save_exit_code(t_cmd *tmp, int *process_exit)
{
	int	r_val;

	r_val = 0;
	if (!tmp || !process_exit)
		return ;
	if (WIFSIGNALED(*process_exit))
		r_val = WTERMSIG(*process_exit) + 128;
	if (WIFEXITED(*process_exit))
		r_val = WEXITSTATUS(*process_exit);
	if (WIFSTOPPED(*process_exit))
		r_val = WSTOPSIG(*process_exit) + 128;
	r_val = normalize_return(r_val);
	update_var(tmp->link, "?", gc_itoa(r_val, tmp->link->gc, 1));
}

int	redirect(t_cmd *cmd)
{
	if (!cmd)
		return (-1);
	if (cmd->input_fd > 0)
		if (dup2(cmd->input_fd, STDIN_FILENO) < 0)
			return (mshell_error(cmd->cmd_name, errno, cmd->link));
	if (cmd->output_fd > 1)
		if (dup2(cmd->output_fd, STDOUT_FILENO) < 0)
			return (mshell_error(cmd->cmd_name, errno, cmd->link));
	return (0);
}

int	normalize_return(int exit_code)
{
	int	normalized;

	normalized = exit_code;
	if (normalized < 0)
		normalized = 256 + normalized;
	if (normalized > 255)
		normalized = normalized % 256;
	return (normalized);
}
