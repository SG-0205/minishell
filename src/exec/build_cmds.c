/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:11:54 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/21 16:14:25 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_cmd	*link_input_pipes(t_cmd **cmds, t_mshell *data)
{
	t_cmd	*prev;
	t_cmd	*tmp;
	int		cmd_id;
	int		lst_size;

	if (!cmds || !*cmds || !data)
		return (NULL);
	lst_size = cmd_list_size(cmds);
	cmd_id = -1;
	tmp = *cmds;
	prev = NULL;
	while (++cmd_id < lst_size && tmp)
	{
		if (prev)
			if (tmp->input_fd < 1)
				tmp->input_fd = prev->pipe_fds[0];
		prev = tmp;
		tmp = tmp->next;
	}
	return (*cmds);
}

t_cmd	*link_output_pipes(t_cmd **cmds, t_mshell *data)
{
	t_cmd	*tmp;
	int		cmd_id;
	int		lst_size;

	if (!cmds || !*cmds || !data)
		return (NULL);
	lst_size = cmd_list_size(cmds);
	cmd_id = -1;
	tmp = *cmds;
	while (++cmd_id < lst_size && tmp)
	{
		if (tmp->output_fd < 2 && cmd_id < lst_size - 1)
			tmp->output_fd = tmp->pipe_fds[1];
		else if (tmp->output_fd < 2 && cmd_id == lst_size - 1)
			tmp->output_fd = 1;
		tmp = tmp->next;
	}
	return (*cmds);
}

t_cmd	*manage_first_input(t_cmd *first_cmd)
{
	if (!first_cmd)
		return (NULL);
	if (first_cmd->input_fd < 1)
		first_cmd->input_fd = 0;
	if (cmd_list_size(&first_cmd) == 1)
	{
		close(first_cmd->pipe_fds[0]);
		close(first_cmd->pipe_fds[1]);
	}
	return (first_cmd);
}

t_cmd	*convert_tokens_and_add_redirections(t_parse *parsing, char	**tokens,
	t_mshell *data)
{
	int		i;
	t_cmd	*cmd_list;
	t_cmd	*new;

	if (!parsing || !tokens || !data)
		return (NULL);
	i = -1;
	cmd_list = NULL;
	while (tokens[++i])
	{
		if (!cmd_list)
			cmd_list = new_cmd_from_tokens(tokens[i], parsing, data);
		else
		{
			new = new_cmd_from_tokens(tokens[i], parsing, data);
			new->prev = last_cmd(&cmd_list);
			last_cmd(&cmd_list)->next = new;
		}
		if (!cmd_list)
			return (NULL);
	}
	return (cmd_list);
}

t_cmd	*build_cmds_list(t_parse *parsing, t_mshell *data)
{
	t_cmd	*cmd_list;
	char	**cmd_tokens;

	if (!parsing || !data)
		return (NULL);
	cmd_tokens = split_input_by_pipes(parsing->input, data);
	if (!cmd_tokens)
		return (NULL);
	if (check_empty_cmd(cmd_tokens, parsing, data) == FALSE)
		return (NULL);
	cmd_list = convert_tokens_and_add_redirections(parsing, cmd_tokens, data);
	if (!cmd_list)
		return (NULL);
	// print_redirection_list(cmd_list->redirs, data);
	return (cmd_list);
}

//VERIFIER . et ..!
