/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:24:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/01 17:34:17 by sgoldenb         ###   ########.fr       */
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

t_cmd	*new_empty_cmd(t_mshell *data)
{
	t_cmd	*new;

	if (!data)
		return (NULL);
	new = (t_cmd *)gc_malloc(data->gc, sizeof(t_cmd), 1);
	if (!new)
		return (NULL);
	new->append_out = FALSE;
	new->args = NULL;
	new->env = NULL;
	new->input_fd = -1;
	new->output_fd = -1;
	new->is_redirected = FALSE;
	new->path_to_cmd = NULL;
	new->pipe_fds = NULL;
	new->next = NULL;
	return (new);
}
