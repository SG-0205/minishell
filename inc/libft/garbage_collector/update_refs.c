/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_refs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:41:34 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/15 22:53:47 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/*Remplace [old_ref] par [new_ref] dans le collecteur
	- Ne crée pas de nouvel élément dans le collecteur.
	- Libère l'ancienne adresse [old_ref].
	- Renvoie l'adresse mise à jour ou NULL en cas de problème.*/
void	*gc_replace_ref(t_collector *gc, void *new_ref, void *old_ref)
{
	t_list	*tmp;

	if (!gc || !old_ref || !gc->ref_layers
		|| !*gc->ref_layers)
		return (NULL);
	tmp = gc_search(old_ref, gc);
	if (tmp)
	{
		free_ref(tmp);
		tmp->content = new_ref;
	}
	else
		return (NULL);
	return (tmp->content);
}

/*Déplace l'élément qui contient [ref] vers la couche [layer].
	- Trouve la référence [ref] à déplacer dans le collecteur.
	- Ajoute l'élément contenant [ref] à la fin de la couche [layer].
	- S'assure du lien entre chaque élément des différentes couches
	- Renvoie 0 en cas de succès*/
int	gc_switch_layer(t_collector *gc, void *ref, size_t layer)
{
	t_list	*tmp;
	t_list	*prev;
	t_list	*next;

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
