/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flush.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:28:12 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 14:40:39 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

static int	clear_collector(t_collector *gc)
{
	if (!gc)
		return (1);
	if (gc->additional_fcts)
		free(gc->additional_fcts);
	if (gc->ref_layers)
		free(gc->ref_layers);
	return (0);
}

/*Libère l'adresse contenue dans l'élément ref
	- Ne libère pas l'élément.
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
	while (tmp)
	{
		tmp2 = tmp->next;
		free_ref(tmp);
		free(tmp);
		tmp = tmp2;
	}
	return (0);
}

/*Remets tous les pointeurs sur les fonctions additionnelles à NULL.
	- Ne libère pas le tableau des pointeurs sur fonction*/
int	gc_flush_fcts(t_collector *gc)
{
	size_t	size;

	if (!gc || !gc->additional_fcts)
		return (EINVAL);
	size = ft_arrlen(gc->additional_fcts);
	while (size--)
		gc->additional_fcts[size] = NULL;
	return (0);
}
