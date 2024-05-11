/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:03:22 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/11 22:32:41 by sgoldenb         ###   ########.fr       */
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

static t_refs	*search_ref(void *ref, t_refs *refs)
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

/*Prend l'adresse du collecteur et la nouvelle ref a ajouter a gc->refs[layer]
N'ajoute pas en cas de doublon et cree le premier element au besoin*/
int	add_ref(t_collector *gc, void *ref, size_t layer)
{
	if (!gc || !ref)
		return (EINVAL);
	if (layer > gc->nb_layers)
		return (EINVAL);
	if (!gc->ref_layers[layer])
	{
		gc->ref_layers[layer] = new_ref(ref);
		if (!gc->ref_layers[layer])
			return (1);
		return (0);
	}
	if (!search_ref(ref, gc->ref_layers[layer]))
	{
		get_last(gc->ref_layers[layer])->next = new_ref(ref);
		gc->nb_refs --;
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
	t_refs	*tmp = collecor->ref_layers[0];
	while (tmp)
		printf("dst : %p\n", tmp->reference), tmp = tmp->next;
	printf("\nTEST GC_MALLOC\n");
	int	*d_test = (int *)gc_malloc(collecor, sizeof(int), 1);
	printf("INT @ %p\n", d_test);
	tmp = collecor->ref_layers[1];
	while (tmp)
		printf("dst+int : %p\n", tmp->reference), tmp = tmp->next;
	t_collector	*c_test = (t_collector *)gc_malloc(collecor, sizeof(t_collector), 1);
	printf("STRUCT @ %p\n", c_test);
	add_ref(collecor, c_test, 1);
	tmp = collecor->ref_layers[1];
	while (tmp)
		printf("dst+int+struct : %p\n", tmp->reference), tmp = tmp->next;
	gc_flush(collecor);
	return (0);
}