/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flush.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:28:12 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/11 22:29:42 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

static int	clear_collector(t_collector *gc)
{
	if (!gc)
		return (1);
	if (gc->additional_fcts)
		ft_arrfree((void **)gc->additional_fcts);
	if (gc->ref_layers)
		ft_arrfree((void **)gc->ref_layers), free(gc->ref_layers);
	return (0);
}

static int	free_ref(t_refs *ref)
{
	if (!ref)
		return (EINVAL);
	if (!ref->reference)
		return (EINVAL);
	free(ref->reference);
	return (0);
}

/*Fait free() sur toutes les adresses et renvoie 0 en cas de succes.*/
int	gc_flush(t_collector *gc)
{
	t_refs	*tmp;
	t_refs	*tmp2;

	if (!gc || !gc->ref_layers)
		return (1);
	while (gc->nb_layers--)
	{
		tmp = gc->ref_layers[gc->nb_layers];
		while (tmp)
		{
			tmp2 = tmp->next;
			free_ref(tmp);
			free(tmp);
			tmp = tmp2;
		}
	}
	clear_collector(gc);
	free(gc);
	return (0);
}

int	gc_flush_layer(t_collector *gc, size_t layer)
{
	t_refs	*tmp;
	t_refs	*tmp2;

	if (!gc)
		return (1);
	tmp = gc->ref_layers[layer];
	while (tmp)
	{
		tmp2 = tmp->next;
		free_ref(tmp);
		free(tmp);
		tmp = tmp2;
	}
	return (0);
}
