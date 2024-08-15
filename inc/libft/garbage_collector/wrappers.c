/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 18:34:04 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/15 22:50:30 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/*Wrapper de malloc, meme comportement.
Ajoute l'adresse du nouvel element a gc->refs[layer] puis retourne l'adresse*/
void	*gc_malloc(t_collector *gc, size_t size, size_t layer)
{
	void	*alloc;

	if (size > __INT_MAX__)
		return (NULL);
	alloc = (void *)malloc(size);
	if (!alloc)
		return (NULL);
	if (!gc)
		return (NULL);
	if (gc_add_ref(gc, alloc, layer) != 0)
		free(alloc);
	return (alloc);
}

/*Libère l'adresse *ptr et tente de l'enlever du collecteur.
	- Renvoie 0 si *ptr est libéré.
	- Renvoie 1 si *ptr est libéré et retiré du collecteur.
	- renvoie -1 si une des opérations free() échoue.*/
int	gc_free(t_collector *gc, void *ptr)
{
	t_list	*tmp;

	if (!gc || !ptr)
		return (-1);
	tmp = gc_search(ptr, gc);
	if (tmp)
	{
		(free_ref(tmp), free(tmp));
		return (1);
	}
	else if (!tmp && ptr)
	{
		free(ptr);
		return (0);
	}
	return (-1);
}
