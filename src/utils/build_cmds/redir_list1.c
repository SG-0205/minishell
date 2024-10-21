/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_list1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:23:40 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/18 13:37:54 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_redirs	*new_redirection(int *fd, int *cmd_id,
	t_redir_type *type, t_mshell *data)
{
	t_redirs	*new;

	if (!cmd_id || !fd)
		return (NULL);
	//printf("N_R fd:%d id:%d\n", *fd, *cmd_id);
	new = (t_redirs *)gc_malloc(data->gc, sizeof(t_redirs), 1);
	if (!new)
		return (NULL);
	//printf("N_R @%p\n", new);
	new->cmd_id = *cmd_id;
	new->fd = *fd;
	new->type = *type;
	new->next = NULL;
	new->path = NULL;
	new->errcorde = 0;
	//printf("N_R @%p\n", new);
	return (new);
}

t_redirs	*dup_redirection(t_redirs **elem_addr, t_mshell *data)
{
	t_redirs	*new;
	t_redirs	*elem;

	if (!elem_addr || !*elem_addr)
		return (NULL);
	elem = *elem_addr;
	new = new_redirection(&elem->fd, &elem->cmd_id, &elem->type, data);
	new->path = gc_strdup(elem->path, data->gc, 1);
	new->errcorde = elem->errcorde;
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

t_redirs	*filter_redirs_by_type(t_redirs **origin, t_redir_type type,
	t_mshell *data)
{
	t_redirs	*sub_list;
	t_redirs	*tmp;
	int			cpy;

	if (!origin || !*origin || (type != INPUT && type != OUTPUT))
		return (NULL);
	sub_list = NULL;
	tmp = *origin;
	while (tmp)
	{
		cpy = 0;
		if (type == INPUT && (tmp->type == INPUT || tmp->type == HEREDOC))
			cpy = 1;
		else if (type == OUTPUT && (tmp->type == OUTPUT || tmp->type == APPEND))
			cpy = 1;
		if (cpy == 1 && !sub_list)
			sub_list = dup_redirection(&tmp, data);
		else if (cpy == 1)
			(get_last_redir(&sub_list)->next = dup_redirection(&tmp, data));
		tmp = tmp->next;
	}
	return (sub_list);
}

t_redirs	*get_last_redir_by_cmd_id(t_redirs **start, int cmd_id)
{
	t_redirs	*tmp;

	if (!start || !*start || cmd_id < 0)
		return (NULL);
	tmp = *start;
	while (tmp->next && tmp->next->cmd_id <= cmd_id)
		tmp = tmp->next;
	if (tmp->cmd_id != cmd_id)
		return (NULL);
	return (tmp);
}

// int	get_cmd_id(char *parsed, char *red_start)
// {
// 	int	i;
// 	int	pipe_count;

// 	if (!parsed || !red_start)
// 		return (-1);
// 	i = -1;
// 	pipe_count = 0;
// 	while (parsed[++i] && &parsed[i] != red_start)
// 	{
// 		if (parsed[i] == '|'
// 			&& is_quoted_by('\'', &parsed[i], parsed) == FALSE
// 			&& is_quoted_by('\"', &parsed[i], parsed) == FALSE)
// 			pipe_count ++;
// 	}
// 	return (pipe_count);
// }
