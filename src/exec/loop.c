/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:19:10 by estegana          #+#    #+#             */
/*   Updated: 2024/10/22 22:37:28 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	close_all_fds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	if (!cmds)
		return (1);
	i = 0;
	tmp = cmds;
	while (tmp
		&& tmp->link->fds[i][0] != -1
		&& tmp->link->fds[i][1] != -1)
	{
		if (tmp->output_fd != 1)
			close(tmp->output_fd);
		if (tmp->input_fd != 0)
			close(tmp->input_fd);
		close(tmp->link->fds[i][1]);
		close(tmp->link->fds[i][0]);
		i ++;
		tmp = tmp->next;
	}
	return (0);
}

t_cmd	*get_first_cmd(t_cmd *end)
{
	t_cmd	*tmp;

	if (!end)
		return (NULL);
	tmp = end;
	while (tmp->prev)
		tmp = tmp->prev;
	return (tmp);
}

void	save_exec_error(t_cmd *cmd)
{
	t_f_check	exec_check;

	if (!cmd)
		return ;
	// if (cmd->exit)
	if (cmd->exit == ENOENT)
		return (update_return_code(1, cmd->link));
	else if (cmd->exit == EACCES)
		return (update_return_code(1, cmd->link));
	exec_check = f_access_check(cmd->cmd_name, NULL);
	if (exec_check.exists == FALSE)
		return (update_return_code(127, cmd->link));
	if (exec_check.exec == FALSE)
		return (update_return_code(1, cmd->link));
	if (exec_check.is_dir == TRUE)
	{
		if (is_relative_path(cmd->cmd_name, cmd->link) == TRUE
			|| ft_strcmp(cmd->cmd_name, get_pwd(cmd->link)) == 0)
			return (update_return_code(126, cmd->link));
		else
			return (update_return_code(127, cmd->link));
	}
}

int	wait_all(t_cmd *start)
{
	t_cmd	*tmp;
	int		stats;

	tmp = NULL;
	stats = 0;
	if (!start)
		return (1);
	tmp = start;
	while (tmp)
	{
		if (tmp->pid > -1)
		{
			waitpid(tmp->pid, &stats, 0);
			save_exit_code(tmp, &stats);
		}
		else
			save_exec_error(tmp);
		tmp = tmp->next;
	}
	return (0);
}

int	exec_b_wrapper(t_cmd *cmd, t_bool is_forked)
{
	int	exit_code;

	exit_code = normalize_return(exec_builtins(
					cmd->args[0], &cmd->args[1], cmd->link));
	update_return_code(exit_code, cmd->link);
	if (is_forked == TRUE)
	{
		clear_data(cmd->link);
		exit(exit_code);
	}
	else
		return (exit_code);
}

int	single_exec(t_cmd *cmd)
{
	int	r_code;

	if (!cmd || !cmd->link || !cmd->args || !cmd->args[0])
		return (close_all_fds(cmd));
	if (is_local(cmd, get_first_cmd(cmd)) == TRUE)
		return (exec_b_wrapper(cmd, FALSE));
	cmd->pid = fork();
	if (cmd->pid < 0)
		return (2);
	if (cmd->pid == 0)
	{
		if (cmd->input_fd != 0)
			dup2(cmd->input_fd, STDIN_FILENO);
		if (cmd->output_fd != 0)
			dup2(cmd->output_fd, STDOUT_FILENO);
		close_all_fds(cmd);
		if (cmd->is_builtin == TRUE)
			exec_b_wrapper(cmd, TRUE);
		else
			if (execve(cmd->args[0], cmd->args, cmd->env) == -1)
				update_return_code(127, cmd->link);
	}
	update_var(cmd->link, "_", cmd->args[0]);
	r_code = wait_all(cmd);
	return (r_code);
}

void	fork_mshell(t_cmd *cmd, int *i)
{
	cmd->pid = fork();
	if (cmd->pid == -1)
		perror("cmd fork failed\n");
	if (cmd->pid == 0)
		ft_child(cmd, i);
}

int	ft_loop(t_cmd *list)
{
	t_cmd	*tmp;
	t_bool	flag;
	int		i;

	if (!list)
		return (0);
	tmp = list;
	i = 0;
	while (tmp)
	{
		flag = try_redirections(tmp, &i);
		if (flag == TRUE && tmp->args && tmp->args[0])
			fork_mshell(tmp, &i);
		if (!(!tmp->next && i == 0))
			ft_parent(tmp, &i);
		if (flag == TRUE && tmp->args)
			update_var(tmp->link, "_", tmp->args[0]);
		tmp = tmp->next;
		i++;
	}
	wait_all(list);
	close_all_fds(list);
	return (0);
}

//si c un builtin qui sort un stdout : ne pas fork
//si c un builtin

//creer une fonction qui va execute le builtin en renvoyant un int
