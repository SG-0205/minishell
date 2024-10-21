/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:24:57 by estegana          #+#    #+#             */
/*   Updated: 2024/10/21 13:44:45 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_cmd	*get_first_item(t_cmd *end)
{
	t_cmd	*tmp;

	if (!end)
		return (NULL);
	tmp = end;
	while (tmp->prev)
		tmp = tmp->prev;
	return (tmp);
}

int	ft_parent(t_cmd *list, int *i)
{
	int		size;
	t_cmd	*list_top;

	if (!list)
		return (1);
	list_top = get_first_item(list);
	size = cmd_list_size(&list_top);
	if (*i == 0)
	{
		close(list->link->fds[*i][1]);//Inutile?
		// close(list->link->fds[*i][0]);
	}
	else if (*i > 0 && *i < size - 1)
	{
		close(list->link->fds[*i - 1][1]); 
		close(list->link->fds[*i - 1][0]);
		// close(list->link->fds[*i][0]);
		close(list->link->fds[*i][1]);
	}
	else if (*i == size - 1)
	{
		close(list->link->fds[*i - 1][1]); 
		close(list->link->fds[*i - 1][0]);
		close(list->link->fds[*i][1]);
		close(list->link->fds[*i][0]);
	}
	return (0);
}
