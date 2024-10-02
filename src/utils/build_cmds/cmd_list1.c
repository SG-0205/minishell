/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:24:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/02 16:22:53 by sgoldenb         ###   ########.fr       */
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

	if (!start || !*start)
		return (0);
	size = 0;
	tmp =  *start;
	while (tmp)
	{
		size ++;
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
	new->args = NULL;
	new->env = NULL;
	new->input_fd = -1;
	new->output_fd = -1;
	new->path_to_cmd = NULL;
	new->is_builtin = 0;
	new->next = NULL;
	if (pipe(new->pipe_fds) == -1)
	{
		mshell_error("pipe error", errno, data);
		update_var(data, "?", gc_itoa(1, data->gc, 1));
		return (NULL);
	}
	return (new);
}
