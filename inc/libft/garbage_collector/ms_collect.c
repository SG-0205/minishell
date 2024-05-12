/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_collect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:30:23 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 09:38:13 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

/*Renvoie l'adresse d'un nouveau collecteur avec [nb_layers] couches.
	- Chaque couche peut enregistrer des references
	- Elles sont initialisees vides
	- Leurs adresses ne sont pas enregistrees dans le collecteur
	- Il n'est pas utile de les referencer (cf gc_flush)*/
t_collector	*gc_init(int nb_layers)
{
	t_collector	*new;
	int			i;

	if (nb_layers > __INT_MAX__ || nb_layers < 1)
		return (NULL);
	new = (t_collector *)malloc(sizeof(t_collector));
	if (!new)
		return (NULL);
	new->nb_layers = nb_layers;
	new->ref_layers = (t_refs **)malloc(sizeof(t_refs) * (nb_layers + 1));
	if (!new->ref_layers)
		return (NULL);
	i = -1;
	new->additional_fcts = NULL;
	new->ref_layers[nb_layers] = NULL;
	while (++i < nb_layers)
		new->ref_layers[i] = NULL;
	new->nb_refs = 0;
	return (new);
}
