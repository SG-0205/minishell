/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:09:06 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/06 08:50:34 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	full_path_len(t_pn *start)
{
	t_pn	*tmp;
	int		total_len;

	if (!start)
		return (0);
	tmp = start;
	total_len = 0;
	while (tmp)
	{
		total_len += ft_strlen(tmp->dir) + 2;
		tmp = tmp->next;
	}
	return (total_len);
}

t_pn	*last_dir(t_pn *start)
{
	t_pn	*tmp;

	if (!start)
		return (NULL);
	tmp = start;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_pn	*new_path_node(char *dir, t_mshell *data)
{
	t_pn	*new;

	if (!dir)
		return (NULL);
	new = (t_pn *)gc_malloc(data->gc, sizeof(t_pn), 1);
	new->next = NULL;
	new->dir = gc_strdup(dir, data->gc, 1);
	if (!new->dir)
		return (NULL);
	return (new);
}

void	add_dir(char *dir, t_pn *list, t_mshell *data)
{
	if (!dir || !list || !data)
		return ;
	last_dir(list)->next = new_path_node(dir, data);
}

void	rm_last_dir(t_pn *start, t_mshell *data)
{
	t_pn	*tmp;
	
	if (!start)
		return ;
	data = data;
	tmp = start;
	printf("RM_DIR :\t[0]%s\t[last]%s\n", start->dir, last_dir(start)->dir);
	if (tmp == last_dir(start))
	{
		tmp->dir = ft_strcpy(tmp->dir, "/\0");
		return ;
	}
	while (tmp->next != last_dir(start))
		tmp = tmp->next;
	tmp->next = NULL;
}
