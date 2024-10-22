/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:22:20 by estegana          #+#    #+#             */
/*   Updated: 2024/10/22 22:36:26 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	clean_forward_fds(t_cmd *cmd, int *i)
{
	int	start;
	int	limit;
	t_cmd	*top;

	if (!cmd)
		return ;
	start = *i - 1;
	top = get_first_item(cmd);
	limit = cmd_list_size(&top);
	while (++start < limit)
		close(cmd->link->fds[start][0]), close(cmd->link->fds[start][1]);
}

void	dup_file_redirs(int todup, t_redir_type i_o)
{
	if (todup < 0 || (i_o != INPUT && i_o != OUTPUT))
		return ;
	if (i_o == INPUT)
		dup2(todup, STDIN_FILENO);
	else if (i_o == OUTPUT)
		dup2(todup, STDOUT_FILENO);
	if (todup > 1)
		close(todup);
		
}

void	mid_redir(t_cmd *cmd, int *i)
{
	if (cmd->input_fd == 0)
	{
		close(cmd->link->fds[*i - 1][1]);
		dup2(cmd->link->fds[*i - 1][0], STDIN_FILENO);
		close(cmd->link->fds[*i - 1][0]);
	}
	else if (cmd->input_fd > 0)
		dup_file_redirs(cmd->input_fd, INPUT);
	if (cmd->output_fd == 1)
	{
		dup2(cmd->link->fds[*i][1], STDOUT_FILENO);
		close(cmd->link->fds[*i][1]);
	}
	else if (cmd->output_fd > 1)
		dup_file_redirs(cmd->output_fd, OUTPUT);
}

void	pipe_redirs(t_cmd *cmd, int *i)
{
	if (*i == 0)
	{
		if (cmd->output_fd == 1 && cmd->next)
			dup2(cmd->link->fds[*i][1], STDOUT_FILENO);
		else if (cmd->output_fd > 1)
			dup_file_redirs(cmd->output_fd, OUTPUT);
		if (cmd->input_fd > 0) 
			dup_file_redirs(cmd->input_fd, INPUT);
	}
	else if (cmd->prev && cmd->next)
		mid_redir(cmd, i);
	else if (!cmd->next)
	{
		if (cmd->input_fd == 0)
		{
			close(cmd->link->fds[*i - 1][1]);
			dup2(cmd->link->fds[*i - 1][0], STDIN_FILENO);
			close(cmd->link->fds[*i - 1][0]);
		}
		else if (cmd->input_fd > 0)
			dup_file_redirs(cmd->input_fd, INPUT);
		if (cmd->output_fd > 1)
			dup_file_redirs(cmd->output_fd, OUTPUT);
	}
}

void	open_heredocs(t_redirs **list, t_mshell *data)
{
	t_redirs	*tmp;

	if (!list || !*list || !data)
		return ;
	tmp = *list;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			tmp->fd = try_open(tmp->path, HEREDOC, data);
		tmp = tmp->next;
	}
}

t_bool	already_checked(t_redirs **list)
{
	t_redirs	*tmp;

	if (!list || !*list)
		return (TRUE);
	tmp = *list;
	while (tmp)
	{
		if (tmp->fd != -2)
			return (TRUE);
		tmp = tmp->next;
	}
	return (FALSE);
}

t_bool	check_bad_redirs(t_redirs **list, t_mshell *data)
{
	t_redirs	*tmp;

	if (!list || !*list || !data)
		return (FALSE);
	if (already_checked(list) == TRUE)
		return (ERROR);
	tmp = *list;
	open_heredocs(list, data);
	while (tmp)
	{
		if (tmp->fd == -2)
		{
			tmp->fd = try_open(tmp->path, tmp->type, data);
			if (tmp->fd == -1)
			{
				tmp->errcorde = errno;
				mshell_error(tmp->path, errno, data);
				return (FALSE);
			}
			tmp->fd = dup(tmp->fd);
		}
		tmp = tmp->next;
	}
	return (TRUE);
}

t_redirs	*get_faulty_redir(t_redirs **list)
{
	t_redirs	*tmp;

	if (!list || !*list)
		return (NULL);
	tmp = *list;
	while (tmp)
	{
		if (tmp->fd == -1)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_redirs	*select_redirection(t_cmd *cmd, t_redir_type i_o)
{
	t_redirs	*filtered_list;
	t_redirs	*final_item;

	if (i_o != INPUT && i_o != OUTPUT)
		return (NULL);
	final_item = NULL;
	filtered_list = NULL;
	if (check_bad_redirs(&cmd->redirs, cmd->link) == FALSE)
	{
		cmd->exit = get_faulty_redir(&cmd->redirs)->errcorde;
		return (get_faulty_redir(&cmd->redirs));
	}
	filtered_list = filter_redirs_by_type(&cmd->redirs, i_o, cmd->link);
	final_item = get_last_redir(&filtered_list);
	if (!final_item)
		return (NULL);
	return (final_item);
}

t_bool	validate_redirection(t_redirs *redir, t_mshell *data)
{
	if (!redir)
		return (FALSE);
	if (redir->fd < 0)
	{
		if (redir->type != HEREDOC)
			mshell_error(redir->path, redir->errcorde, data);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	try_redirections(t_cmd *cmd, int *i)
{
	t_redirs	*input;
	t_redirs	*output;

	if (!cmd->redirs || has_redirs_by_id(i, cmd->redirs) == FALSE)
		return (TRUE);
	cmd->redirs = filter_redirs_by_id(&cmd->redirs, i, cmd->link);
	input = select_redirection(cmd, INPUT);
	output = select_redirection(cmd, OUTPUT);
	if (!input && !output)
		return (TRUE);
	if ((input && input->fd < 0) || (output && output->fd < 0))
		return (FALSE);
	if (input)
		cmd->input_fd = input->fd;
	if (output)
		cmd->output_fd = output->fd;
	// printf("OUT[%d]\t%d -> %d[%s]\n", *i, cmd->output_fd, output->fd, output->path);
	// printf("IN[%d]\t%d -> %d[%s]\n", *i, cmd->input_fd, input->fd, input->path);
	return (TRUE);
}

void	simple_exec(t_cmd *cmd)
{
	int	exit_code;

	if (!cmd)
		return ;
	exit_code = 0;
	if (cmd->input_fd > 0)
		dup_file_redirs(cmd->input_fd, INPUT);
	if (cmd->output_fd > 1)
		dup_file_redirs(cmd->output_fd, OUTPUT);
	close(cmd->link->fds[0][0]);
	close(cmd->link->fds[0][1]);
	if (cmd->is_builtin == TRUE)
	{
		exit_code = exec_builtins(cmd->args[0], &cmd->args[1], cmd->link);
		clear_data(cmd->link);
		exit(normalize_return(exit_code));
	}
	else
		ft_execute(cmd);
}

int	ft_child(t_cmd *list, int *i)
{
	int	exit_code;

	if (!list->next && *i == 0)
		simple_exec(list);
	else if (list->is_builtin)
	{
		exit_code = 0;
		pipe_redirs(list, i);
		clean_forward_fds(list, i);
		exit_code = exec_builtins(list->args[0], &list->args[1], list->link);
		clear_data(list->link);
		exit(normalize_return(exit_code));
	}
	else
	{
		pipe_redirs(list, i);
		clean_forward_fds(list, i);
		ft_execute(list);
	}
	return (0);
}

//VERIFIER AVEC LES REDIRECTIONS, NE DOIS PAS PIPE SI REDIRS OUT ET IDEM POUR INPUT