/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_refs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 14:34:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/08 15:02:32 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include <stdio.h>

static t_list	*new_ref(void *content)
{
	t_list	*new;

	if (!content)
		return (NULL);
	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

/*Renvoie le dernier element d'une couche du collecteur.*/
t_list	*gc_get_last(t_list *start)
{
	t_list	*tmp;

	if (!start)
		return (NULL);
	tmp = start;
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
		gc_get_last(gc->ref_layers[layer])->next = new_ref(ref);
		gc->nb_refs++;
	}
	return (0);
}

// int	main(int argc, char **argv)
// {
// 	t_collector *gc;
// 	if (argc != 6)
// 		return (1);
// 	(void)argv;
// 	gc = gc_init(2);
// 	if (!gc || !gc->ref_layers)
// 		return (1);
// 	char *test_c1 = (char *)gc_malloc(gc, sizeof(char), 0);
// 	*test_c1 = 'L';
// 	int *d_test = (int *)gc_malloc(gc, sizeof(int), 0);
// 	*d_test = 5;
// 	t_collector *c_test = (t_collector *)gc_malloc(gc,
// 		sizeof(t_collector), 0);
// 	gc_split("123:456:789", ':', gc, 0);
// 	gc_print_layers(gc);
// 	gc_flush(gc);
// 	return (0);
// }