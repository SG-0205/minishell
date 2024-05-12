/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_refs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 14:34:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 18:59:40 by sgoldenb         ###   ########.fr       */
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

	if (!refs)
		return (NULL);
	tmp = refs;
	while (tmp)
	{
		if (tmp->next == NULL)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
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
		gc->nb_refs++;
	}
	return (0);
}

void	*test(void **ptr)
{
	*ptr = NULL;
	return (*ptr);
}
void	*test2(void **ptr)
{
	*ptr = NULL;
	return (*ptr);
}
int	main(int argc, char **argv)
{
	t_collector *gc;

	if (argc != 6)
		return (1);
	(void)argv;
	gc = gc_init(2);
	gc_init_fcts(gc, 2);
	if (!gc || !gc->ref_layers)
		return (1);
	char *test_c1 = (char *)gc_malloc(gc, sizeof(char), 0);
	*test_c1 = 'L';
	int *d_test = (int *)gc_malloc(gc, sizeof(int), 1);
	*d_test = 5;
	t_collector *c_test = (t_collector *)gc_malloc(gc,
		sizeof(t_collector), 1);
	gc_add_ref(gc, c_test, 1);
	gc_add_fct(gc, test);
	gc_add_fct(gc, test2);
	gc_print(gc);
	gc_fct_on_layer(gc, 1, 0);
	gc_print_layers(gc);
	gc_flush_fcts(gc);
	gc_print(gc);
	gc_flush(gc);
	return (0);
}
