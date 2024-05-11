/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:03:22 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/11 17:02:16 by sgoldenb         ###   ########.fr       */
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

int	add_ref(t_collector *gc, void *ref, int layer)
{
	if (!gc || !ref)
		return (EINVAL);
	if (layer > gc->nb_layers || layer < 0)
		return (EINVAL);
	if (!gc->ref_layers[layer])
	{
		gc->ref_layers[layer] = new_ref(ref);
		if (!gc->ref_layers[layer])
			return (1);
		return (0);
	}
	get_last(gc->ref_layers[layer])->next = new_ref(ref);
	if (!gc->ref_layers[layer])
		return (1);
	return (0);
}

// int	main(int argc, char **argv)
// {
// 	t_collector	*collecor;

// 	if (argc != 6)
// 		return (1);
// 	collecor = gc_init(1);
// 	if (!collecor)
// 		return (1);
// 	for (int i = 0; argv[i]; i ++)
// 		add_ref(collecor , argv[i], 0), printf("src : %p\n", argv[i]);
// 	t_refs	*tmp = collecor->ref_layers[0];
// 	while (tmp)
// 		printf("dst : %p\n", tmp->reference), tmp = tmp->next;
// 	return (0);
// }