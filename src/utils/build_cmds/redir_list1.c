/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_list1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:23:40 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/25 19:44:51 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_redirs	*new_redirection(int *fd, int *cmd_id,
	t_redir_type *type, t_mshell *data)
{
	t_redirs	*new;

	if (!cmd_id || !fd)
		return (NULL);
	new = (t_redirs *)gc_malloc(data->gc, sizeof(t_redirs), 1);
	if (!new)
		return (NULL);
	new->cmd_id = *cmd_id;
	new->fd = *fd;
	new->type = *type;
	new->next = NULL;
	return (new);
}

t_redirs	*get_last_redir(t_redirs **start)
{
	t_redirs	*tmp;

	if (!start || !*start)
		return (NULL);
	tmp = *start;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

int	get_cmd_id(char *parsed, char *red_start)
{
	int	i;
	int	pipe_count;

	if (!parsed || !red_start)
		return (-1);
	i = -1;
	pipe_count = 0;
	while (parsed[++i] && &parsed[i] != red_start)
	{
		if (parsed[i] == '|'
			&& is_quoted_by('\'', &parsed[i], parsed) == FALSE
			&& is_quoted_by('\"', &parsed[i], parsed) == FALSE)
			pipe_count ++;
	}
	return (pipe_count);
}
