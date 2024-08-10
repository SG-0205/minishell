/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flush.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:28:12 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/10 13:35:36 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

static int	clear_collector(t_collector *gc)
{
	if (!gc)
		return (1);
	if (gc->ref_layers)
		free(gc->ref_layers);
	return (0);
}

/*Libère l'adresse contenue dans l'élément ref
	- Ne libère pas l'élément.
	- Ne modifie pas l'ordre dans le collecteur.
	- Renvoie 0 si succès et EINVAL si l'adresse est déjà libérée.*/
int	free_ref(t_refs *ref)
{
	if (!ref)
		return (EINVAL);
	if (!ref->reference)
		return (EINVAL);
	free(ref->reference);
	return (0);
}

/*Libère toutes les références avec free()
	- Libère tous les pointeurs en premier.
	- Libère les éléments du collecteur.
	- Libère la structure du collecteur.
	- Renvoie 0 si tout à été libéré correctement.*/
int	gc_flush(t_collector *gc)
{
	t_refs	*tmp;
	t_refs	*tmp2;
	int		i;

	i = -1;
	if (!gc || !gc->ref_layers)
		return (1);
	while (++i < gc->nb_layers)
		gc_flush_layer(gc, i);
	if (clear_collector(gc) == 0)
		free(gc);
	if (!gc)
		return (0);
	else
		return (1);
}

/*Libère les addresses contenues dans la couche [layer]
	- Utilise free() sur toutes les références de la liste
	- Libère aussi chaque élément de la liste
	- Ne libère pas le tableau contenant les couches*/
int	gc_flush_layer(t_collector *gc, size_t layer)
{
	t_refs	*tmp;
	t_refs	*tmp2;

	if (!gc)
		return (1);
	tmp = gc->ref_layers[layer];
	while (tmp && tmp->next)
	{
		tmp2 = tmp->next;
		if (tmp->reference)
			free_ref(tmp);
		free(tmp);
		tmp = tmp2;
	}
	free(gc->ref_layers);
	return (0);
}

