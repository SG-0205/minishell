/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:24:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/18 16:00:37 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_cmd	*last_cmd(t_cmd **start)
{
	t_cmd	*tmp;

	if (!start || !*start)
		return (NULL);
	tmp = *start;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

int	cmd_list_size(t_cmd **start)
{
	int		size;
	t_cmd	*tmp;

	size = 0;
	tmp = NULL;
	if (!start || !*start)
		return (0);
	tmp = *start;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}

t_cmd	*new_empty_cmd(t_mshell *data)
{
	t_cmd	*new;

	if (!data)
		return (NULL);
	new = (t_cmd *)gc_malloc(data->gc, sizeof(t_cmd), 1);
	if (!new)
		return (NULL);
	new->cmd_name = NULL;
	new->exit = 0;
	new->args = NULL;
	new->env = NULL;
	new->args = NULL;
	new->path_to_cmd = NULL;
	new->input_fd = 0;
	new->output_fd = 1;
	new->cmd_name = NULL;
	new->redirs = NULL;
	new->pid = -1;
	new->is_builtin = 0;
	new->next = NULL;
	new->prev = NULL;
	new->link = data;
	new->pipe_fds[0] = 0;
	new->pipe_fds[1] = 0;
	return (new);
}

t_cmd	*select_tokens_and_pass_env(t_cmd *cmd, char **tokens, t_mshell *data)
{
	if (!cmd || !data)
		return (NULL);
	if (tokens)
	{
		if (is_builtin(tokens, cmd, data) == FALSE)
		{
			if (tokens[0])
				cmd->cmd_name = gc_strdup(tokens[0], data->gc, 1);
			if (tokens[0] && !cmd->cmd_name)
				return (NULL);
			tokens[0] = interpolate_path(*tokens, data);
		}
		else if (is_builtin(tokens, cmd, data) == TRUE)
			cmd->cmd_name = gc_strdup(*tokens, data->gc, 1);
		cmd->args = dup_cmd_args(tokens, data);
	}
	cmd->env = env_list_to_array(data, FALSE);
	return (cmd);
}

t_cmd	*new_cmd_from_tokens(char *tokens, t_parse *parsing, t_mshell *data)
{
	t_cmd	*new_cmd;
	char	**splitted_cmds;

	if (!tokens || !parsing || !data)
		return (NULL);
	new_cmd = new_empty_cmd(data);
	if (!new_cmd)
		return (NULL);
	splitted_cmds = initial_split(tokens, data);
	new_cmd = select_tokens_and_pass_env(new_cmd, splitted_cmds, data);
	if (!new_cmd)
		return (NULL);
	// new_cmd = apply_redirections(new_cmd, parsing, cmd_id, data);
	new_cmd->redirs = parsing->redirections;
	if (!new_cmd)
		return (NULL);
	return (new_cmd);
}
