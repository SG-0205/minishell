/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:22:20 by estegana          #+#    #+#             */
/*   Updated: 2024/10/21 14:56:02 by sgoldenb         ###   ########.fr       */
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
	if (i_o == INPUT && todup != 0)
		dup2(todup, STDIN_FILENO);
	else if (i_o == OUTPUT && todup != 1)
		dup2(todup, STDOUT_FILENO);
	close(todup);
}

void	mid_redir(t_cmd *cmd, int *i)
{
	if (cmd->input_fd == 0)
	{
		close(cmd->link->fds[*i - 1][1]);
		dup2(cmd->link->fds[*i - 1][0], STDIN_FILENO);
		// dprintf(STDOUT_FILENO, "%s dup2(%d, %d)\n", cmd->args[0], cmd->link->fds[*i - 1][0], STDIN_FILENO);
		close(cmd->link->fds[*i - 1][0]);
	}
	else
		dup_file_redirs(cmd->input_fd, INPUT);
	if (cmd->output_fd == 1)
	{
		// close(cmd->link->fds[*i][0]);
		dup2(cmd->link->fds[*i][1], STDOUT_FILENO);
		// dprintf(STDOUT_FILENO, "%s dup2(%d, %d)\n", cmd->args[0], cmd->link->fds[*i][1], STDOUT_FILENO);
		close(cmd->link->fds[*i][1]);
	}
	else
		dup_file_redirs(cmd->output_fd, OUTPUT);
}

void	pipe_redirs(t_cmd *cmd, int *i)
{
//
	if (*i == 0)
	{
		// printf("%s is start\n", cmd->args[0]);
		if (cmd->output_fd == 1)
		{
			// dprintf(STDOUT_FILENO, "%s dup2(%d, %d)\n", cmd->args[0], cmd->link->fds[*i][1], STDOUT_FILENO);
			dup2(cmd->link->fds[*i][1], STDOUT_FILENO);
			// close(cmd->link->fds[*i][1]);
			// close(cmd->link->fds[*i][0]);
		}
		else
			dup_file_redirs(cmd->output_fd, OUTPUT);
		if (cmd->input_fd != 0) 
			dup_file_redirs(cmd->input_fd, INPUT);
	}
	else if (cmd->prev && cmd->next)
	{
		// printf("%s is middle\n", cmd->args[0]);
		mid_redir(cmd, i);
	}
	else if (!cmd->next)
	{
		// printf("%s is end\n", cmd->args[0]);
		if (cmd->input_fd == 0)
		{
			close(cmd->link->fds[*i - 1][1]);
			dup2(cmd->link->fds[*i - 1][0], STDIN_FILENO);
			// dprintf(STDOUT_FILENO, "%s dup2(%d, %d)\n", cmd->args[0], cmd->link->fds[*i - 1][0], STDIN_FILENO);
			close(cmd->link->fds[*i - 1][0]);
		}
		else
			dup_file_redirs(cmd->input_fd, INPUT);
		if (cmd->output_fd != 1)
			dup_file_redirs(cmd->output_fd, OUTPUT);
		// close(cmd->link->fds[*i][0]);
		// close(cmd->link->fds[*i][1]);
	}
}

// void	try_redirections(t_cmd *cmd, int *i)
// {
// 	t_redirs	*filtered;
	
// }

t_redirs	*select_redirection(t_cmd *cmd, t_redir_type i_o, int *i)
{
	t_redirs	*filtered_list;
	t_redirs	*final_item;

	if ((i_o != INPUT && i_o != OUTPUT))
		return (NULL);
	final_item = NULL;
	filtered_list = filter_redirs_by_type(&cmd->redirs, i_o, cmd->link);
	if (!filtered_list)
		return (NULL);
	final_item = get_last_redir_by_cmd_id(&filtered_list, *i);
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
		// printf("\nREDIR FAULT[] @%p\t%s\n", redir, redir->path);
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

	if (!cmd->args || !cmd->args[0])
		return (FALSE);
	if (!cmd->redirs || has_redirs_by_id(i, cmd->redirs) == FALSE)
		return (TRUE);
	input = select_redirection(cmd, INPUT, i);
	output = select_redirection(cmd, OUTPUT, i);
	if (!input && !output)
		return (TRUE);
	if (input)
	{
		if (validate_redirection(input, cmd->link) == FALSE)
			return (FALSE);
		cmd->input_fd = input->fd;
	}
	if (output)
	{
		if (validate_redirection(output, cmd->link) == FALSE)
			return (FALSE);
		cmd->output_fd = output->fd;
	}
	return (TRUE);
}

//Tenter de close les pipes fd correspondants auc redirections si presentes

int	ft_child(t_cmd *list, int *i)
{
	int	exit_code;
	if (list->is_builtin)
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