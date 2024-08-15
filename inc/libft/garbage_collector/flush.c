/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flush.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:28:12 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/15 21:57:46 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

static int	clear_collector(t_collector *gc)
{
	if (!gc)
		return (1);
	if (gc->ref_layers)
		free(gc->ref_layers);
	return (0);
}

/*Libère l'adresse contenue dans l'élément ref
	- Ne libère pas l'élément.
	- Ne modifie pas l'ordre dans le collecteur.
	- Renvoie 0 si succès et EINVAL si l'adresse est déjà libérée.*/
int	free_ref(t_list *ref)
{
	if (!ref)
		return (EINVAL);
	if (!ref->content)
		return (EINVAL);
	free(ref->content);
	return (0);
}

/*Libère toutes les références avec free()
	- Libère tous les pointeurs en premier.
	- Libère les éléments du collecteur.
	- Libère la structure du collecteur.
	- Renvoie 0 si tout à été libéré correctement.*/
int	gc_flush(t_collector *gc)
{
	int		i;

	i = -1;
	if (!gc || !gc->ref_layers)
		return (1);
	while (++i < (int)gc->nb_layers)
		gc_flush_layer(gc, i);
	if (clear_collector(gc) == 0)
		free(gc);
	if (!gc)
		return (0);
	else
		return (1);
}

/*Libère les addresses contenues dans la couche [layer]
	- Utilise free() sur toutes les références de la liste
	- Libère aussi chaque élément de la liste
	- Ne libère pas le tableau contenant les couches*/
void	gc_flush_layer(t_collector *gc, size_t layer)
{
	if (!gc || !gc->ref_layers[layer])
		return ;
	ft_lstclear(&gc->ref_layers[layer], free);
}
