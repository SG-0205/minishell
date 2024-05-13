/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_refs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:41:34 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/13 14:56:19 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/*Applique la fonction désignée par [fct_id] sur toute la couche [layer].
	- Ne modifie pas l'ordre des éléments du collecteur.
	- Applique la fonction sur chaque adresse contenue dans chaque élément
	- Renvoie 0 si tout se déroule normalement*/
int	gc_fct_on_layer(t_collector *gc, size_t layer, size_t fct_id)
{
	t_refs	*tmp;

	if (!gc || layer >= gc->nb_layers || fct_id >= gc->nb_fcts
		|| !gc->ref_layers[layer])
		return (1);
	tmp = gc->ref_layers[layer];
	while (tmp)
	{
		if (gc_do_fct(gc, fct_id, tmp->reference) != 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/*Applique la fonction désignée par [fct_id].
	- Ne modifie pas l'ordre des éléments du collecteur.
	- Applique la fonction sur chaque adresse enregistrée dans le collecteur
	- S'arrête et renvoie 1 si l'application ne s'est pas passée comme prévu
	- Dans ce cas, utiliser gc_print() identifier l'adresse responsable.
	- Renvoie 0 en cas de succès*/
int	gc_fct_all_refs(t_collector *gc, size_t fct_id)
{
	int	layers;

	if (!gc || fct_id >= gc->nb_fcts || !gc->ref_layers)
		return (1);
	layers = -1;
	while (gc->ref_layers[++layers])
		if (gc_fct_on_layer(gc, layers, fct_id) != 0)
			return (1);
	return (0);
}

/*Remplace [old_ref] par [new_ref] dans le collecteur
	- Ne crée pas de nouvel élément dans le collecteur.
	- Libère l'ancienne adresse [old_ref].
	- Renvoie l'adresse mise à jour ou NULL en cas de problème.*/
void	*gc_replace_ref(t_collector *gc, void *new_ref, void *old_ref)
{
	t_refs	*tmp;

	if (!gc || !old_ref || !gc->ref_layers
		|| !*gc->ref_layers)
		return (NULL);
	tmp = gc_search(old_ref, gc);
	if (tmp)
	{
		free_ref(tmp);
		tmp->reference = new_ref;
	}
	else
		return (NULL);
	return (tmp->reference);
}

/*Déplace l'élément qui contient [ref] vers la couche [layer].
	- Trouve la référence [ref] à déplacer dans le collecteur.
	- Ajoute l'élément contenant [ref] à la fin de la couche [layer].
	- S'assure du lien entre chaque élément des différentes couches
	- Renvoie 0 en cas de succès*/
int	gc_switch_layer(t_collector *gc, void *ref, size_t layer)
{
	t_refs	*tmp;
	t_refs	*prev;
	t_refs	*next;

	if (!gc || !ref || layer >= gc->nb_layers)
		return (1);
	else if (get_layer(gc, ref) == gc->ref_layers[layer])
		return (1);
	tmp = gc_search(ref, gc);
	if (!tmp)
		return (1);
	prev = get_side_ref(tmp, get_layer(gc, ref), PREV);
	next = get_side_ref(tmp, get_layer(gc, ref), NEXT);
	if (prev && next)
		prev->next = next;
	else if (!prev && next)
		gc->ref_layers[layer] = next;
	else if (prev && !next)
		prev->next = NULL;
	gc_get_last(gc->ref_layers[layer])->next = tmp;
	gc_get_last(gc->ref_layers[layer])->next = NULL;
	return (0);
}
