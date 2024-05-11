/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 18:34:04 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/11 22:25:17 by sgoldenb         ###   ########.fr       */
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
	if (add_ref(gc, alloc, layer) != 0)
		free(alloc);
	return (alloc);
}
