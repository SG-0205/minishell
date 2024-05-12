/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_refs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 09:58:39 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 10:28:40 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/*Cherche ref dans la liste refs
	- Renvoie l'adresse de l'élément de liste si ref est trouvé.
	- Renvoie NULL dans le cas contraire.*/
t_refs	*search_ref(void *ref, t_refs *refs)
{
	t_refs	*tmp;

	if (!ref || !refs)
		return (NULL);
	tmp = refs;
	while (tmp)
	{
		if (tmp->reference == ref)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

/*Cherche ref sur toutes les couches du collecteur
	- Renvoie l'adresse de l'élément contenant ref.
	- Renvoie NULL si ref n'est pas indexé*/
t_refs	*gc_search(void *ref, t_collector *gc)
{
	size_t	nb_layers;
	t_refs	*tmp;

	if (!ref || !gc)
		return (NULL);
	tmp = NULL;
	nb_layers = gc->nb_layers;
	while (nb_layers--)
	{
		tmp = search_ref(ref, gc->ref_layers[nb_layers]);
		if (tmp)
			return (tmp);
	}
	return (NULL);
}
