/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:56:56 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/15 22:50:30 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

t_list	*get_bflast(t_list *refs)
{
	t_list	*tmp;

	if (!refs)
		return (NULL);
	tmp = refs;
	while (tmp->next)
	{
		if (tmp->next->next == NULL)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}

t_list	*get_side_ref(t_list *target, t_list *lst_start, int side)
{
	t_list	*tmp;

	if (!target || !lst_start || (side != PREV && side != NEXT))
		return (NULL);
	tmp = lst_start;
	while (tmp)
	{
		if (side == NEXT && tmp == target)
			return (tmp->next);
		else if (side == PREV && tmp->next == target)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}

/*Renvoie le premier element de la couche qui contient [ref].*/
t_list	*get_layer(t_collector *gc, void *ref)
{
	size_t	nb_layers;
	t_list	*tmp;

	if (!ref || !gc)
		return (NULL);
	tmp = NULL;
	nb_layers = gc->nb_layers;
	while (nb_layers--)
	{
		tmp = search_ref(ref, gc->ref_layers[nb_layers]);
		if (tmp)
			return (gc->ref_layers[nb_layers]);
	}
	return (NULL);
}
