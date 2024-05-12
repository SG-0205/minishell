/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_fcts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 13:03:47 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 14:42:09 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/*Initialise le tableau de fonctions supplémentaires de taille [nb_fcts].
	- Renvoie 0 en cas de succès.
	- Le tableau est initialisé vide.
	- Il peut être mis à jour avec gc_add_fct() ou gc_rm_fct().*/
int	gc_init_fcts(t_collector *gc, size_t nb_fcts)
{
	size_t	i;

	if (nb_fcts == 0 || !gc)
		return (1);
	gc->nb_fcts = nb_fcts;
	i = 0;
	gc->additional_fcts = (void **)malloc(sizeof(void *) * (nb_fcts + 1));
	while (i++ < nb_fcts)
		gc->additional_fcts[i] = NULL;
	if (!gc->additional_fcts)
		return (-1);
	return (0);
}

int	gc_add_fct(t_collector *gc, void (*f)(void *))
{
	size_t	len;

	if (!gc || !f || !gc->additional_fcts)
		return (EINVAL);
	len = (size_t)ft_arrlen(gc->additional_fcts);
	if (len == gc->nb_fcts)
		return (ENOMEM);
	gc->additional_fcts[len] = f;
	return (0);
}
