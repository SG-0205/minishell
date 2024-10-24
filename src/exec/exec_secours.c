/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_secours.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 22:16:38 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/23 10:19:02 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	close_non_pipes_fds(t_cmd *start)
{
	t_cmd	*tmp;
	t_cmd	*prev;

	if (!start)
		return ;
	tmp = start;
	prev = NULL;
	while (tmp)
	{
		if ((tmp->input_fd > 0 && prev && prev->pipe_fds[0] != tmp->input_fd)
			|| (tmp->input_fd > 0 && !prev))
			close(tmp->input_fd);
		if ((tmp->output_fd > 1 && tmp->output_fd != tmp->pipe_fds[1]))
			close(tmp->output_fd);
		prev = tmp;
		tmp = tmp->next;
	}
}

int	exec_builtins(char *b_name, char **b_args, t_mshell *data)
{
	if (ft_strcmp(b_name, "export") == 0)
		return (export(data, b_args));
	else if (ft_strcmp(b_name, "cd") == 0)
		return (cd(b_args, data));
	else if (ft_strcmp(b_name, "env") == 0)
		return (print_env(data, PUBLIC_VARS));
	else if (ft_strcmp(b_name, "unset") == 0)
		return (unset(b_args, data));
	else if (ft_strcmp(b_name, "exit") == 0)
		return (exit_b(b_args, data));
	else if (ft_strcmp(b_name, "pwd") == 0)
		return (pwd(b_args, data));
	else if (ft_strcmp(b_name, "echo") == 0)
		return (echo_b(b_args, data));
	return (-1);
}

int	child_process(t_cmd *cmd)
{
	int	b_exit_code;

	if (!cmd)
		return (-1);
	if (redirect(cmd) != 0)
		return (-1);
	if (cmd->is_builtin == FALSE)
	{
		if (execve(cmd->args[0], cmd->args, cmd->env) == -1)
			mshell_error(cmd->cmd_name, errno, cmd->link);
	}
	else
	{
		b_exit_code = 0;
		b_exit_code = exec_builtins(cmd->args[0], &cmd->args[1], cmd->link);
		clear_data(cmd->link);
		exit(normalize_return(b_exit_code));
	}
	return (-1);
}

void	extern_exec(t_cmd *tmp, t_cmd *prev)
{
	pid_t	pid;
	int		process_exit;

	if (!tmp || !tmp->link)
		return ;
	pid = fork();
	if (pid < 0)
		mshell_error(tmp->cmd_name, errno, tmp->link);
	if (pid == 0)
		child_process(tmp);
	else
		waitpid(0, &process_exit, WUNTRACED);
	if (prev)
		close(prev->pipe_fds[0]);
	close(tmp->pipe_fds[1]);
	save_exit_code(tmp, &process_exit);
}

void	exec_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	t_cmd	*prev;

	if (!cmds || !cmds->link || cmds->link->no_exec == TRUE)
		return ;
	tmp = cmds;
	prev = NULL;
	while (tmp)
	{
		if (local_exec(tmp, cmds) == TRUE || !tmp->args
			|| !tmp->args[0] || ft_isascii(*tmp->args[0]) == 0)
		{
			if (prev)
				close(prev->pipe_fds[0]);
			close(tmp->pipe_fds[1]);
		}
		else
			extern_exec(tmp, prev);
		prev = tmp;
		tmp = tmp->next;
	}
	if (prev)
		close(prev->pipe_fds[0]);
	close_non_pipes_fds(cmds);
}

//Utiliser prev pour fermer l'input precedente du pipe OK
//Skip les commandes vides
//Implementer SIGINT OK
//CLOSE UNUSED FDS -> redirections (cancel @ end) OK