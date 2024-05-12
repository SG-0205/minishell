/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_fcts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 13:03:47 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 17:07:01 by sgoldenb         ###   ########.fr       */
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
	gc->additional_fcts = (t_fct_ptr *)malloc(sizeof(t_fct_ptr) * (nb_fcts
			+ 1));
	while (i++ < nb_fcts)
		gc->additional_fcts[i] = NULL;
	if (!gc->additional_fcts)
		return (-1);
	return (0);
}

/*Ajoute une fonction au tableau du collecteur.
	- Les fonctions doivent être de type void *(*f)(void **).
	- Elles prennent L'ADRESSE D'UN POINTEUR en argument.
	- Elles renvoient un pointeur.
	- Les fonctions sont enregistrées de 0 à gc->nb_fcts dans le tableau.
	- Elles suivent l'ordre des ajouts dans le code.
	- Elles peuvent être appelées à l'aide de leur id (cf. do_fcts()).*/
int	gc_add_fct(t_collector *gc, void *(*f)(void **))
{
	size_t	len;

	if (!gc || !f || !gc->additional_fcts)
		return (EINVAL);
	len = (size_t)ft_arrlen((void **)gc->additional_fcts);
	if (len == gc->nb_fcts)
		return (ENOMEM);
	gc->additional_fcts[len] = f;
	return (0);
}

/*Applique une fonction à une adresse référencées dans le collecteur
	- La fonction à appeler est identifiée par son ordre d'ajout.
	- L'élément contenant [ref] est cherché dans le collecteur.
	- La fonction prend [ref] en paramètre.
	- Retourne 0 en cas de succès, 1 si l'adresse n'est pas dans le collecteur.
	- Aucun changement dans l'ordre des éléments du collecteur*/
int	gc_do_fct(t_collector *gc, size_t fct_id, void *ref)
{
	t_refs	*tmp;

	if (!gc || fct_id >= gc->nb_fcts || !ref)
		return (EINVAL);
	tmp = gc_search(ref, gc);
	if (tmp && gc->additional_fcts[fct_id])
		tmp->reference = gc->additional_fcts[fct_id](&tmp->reference);
	else
		return (1);
	return (0);
}
