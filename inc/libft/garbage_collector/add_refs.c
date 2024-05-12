/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_add_refs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:03:22 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 10:08:33 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include <stdio.h>

static t_refs	*new_ref(void *reference)
{
	t_refs	*new;

	if (!reference)
		return (NULL);
	new = (t_refs *)malloc(sizeof(t_refs));
	new->reference = reference;
	new->next = NULL;
	return (new);
}

static t_refs	*get_last(t_refs *refs)
{
	t_refs	*tmp;

	tmp = refs;
	while (tmp)
	{
		if (tmp->next == NULL)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

/*Ajoute une nouvelle référence au collecteur
	- La nouvelle adresse est ajoutée en dernier sur la couche [layer].
	- Si la couche est vide, crée le premier élément.
	- N'ajoute pas l'adresse si elle est déjà présente dans le collecteur.
	- Renvoie 0 en cas de succès.*/
int	gc_add_ref(t_collector *gc, void *ref, size_t layer)
{
	if (!gc || !ref)
		return (EINVAL);
	if (layer > gc->nb_layers)
		return (EINVAL);
	if (gc_search(ref, gc))
		return (0);
	else if (!gc->ref_layers[layer])
	{
		gc->ref_layers[layer] = new_ref(ref);
		if (!gc->ref_layers[layer])
			return (1);
		return (0);
	}
	else
	{
		get_last(gc->ref_layers[layer])->next = new_ref(ref);
		gc->nb_refs ++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_collector	*collecor;

	if (argc != 6)
		return (1);
	(void)argv;
	collecor = gc_init(2);
	if (!collecor || !collecor->ref_layers)
		return (1);
	char	*test_c1 = (char *)gc_malloc(collecor, sizeof(char), 0);
	*test_c1 = 'L';
	gc_add_ref(collecor, test_c1, 1);
	int	*d_test = (int *)gc_malloc(collecor, sizeof(int), 1);
	*d_test = 5;
	t_collector	*c_test = (t_collector *)gc_malloc(collecor, sizeof(t_collector), 1);
	gc_add_ref(collecor, c_test, 1);
	gc_print_layers(collecor);
	gc_flush(collecor);
	return (0);
}