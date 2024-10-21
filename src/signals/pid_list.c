/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:42:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/08 12:54:34 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_pidlist	*new_pid(pid_t process_pid, t_mshell *data)
{
	t_pidlist	*new;

	if (!data)
		return (NULL);
	new = (t_pidlist *)gc_malloc(data->gc, sizeof(t_pidlist), 2);
	if (!new)
		return (NULL);
	new->next = NULL;
	new->pid = process_pid;
	return (new);
}

t_pidlist	*last_pid(t_pidlist **first_pid)
{
	t_pidlist	*tmp;

	if (!first_pid || !*first_pid)
		return (NULL);
	tmp = *first_pid;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

static void	kill_all(t_pidlist **start, int signal, t_mshell *data)
{
	t_pidlist	*tmp;

	if (!start || !*start)
		return ;
	tmp = *start;
	update_var(data, "?", gc_itoa(128 + signal, data->gc, 2));
	while (tmp)
	{
		kill(tmp->pid, signal);
		tmp = tmp->next;
	}
}

int	kill_processes(int signal, t_mshell *data)
{
	if (!data)
		return (-1);
	if (!data->pids)
		return (1);
	kill_all(&data->pids, signal, data);
	data->pids = NULL;
	gc_flush_layer(data->gc, 2);
	printf(BLUE "[LVL 2 DATA CLEARED]\n" RESET);
	return (0);
}

int	save_pid(pid_t process_pid, t_mshell *data)
{
	t_pidlist	*new;

	if (process_pid < 0 || !data)
		return (1);
	new = new_pid(process_pid, data);
	if (!new)
		return (2);
	if (!data->pids)
		data->pids = new;
	else
		last_pid(&data->pids)->next = new;
	return (0);
}
