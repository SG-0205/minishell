/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_fcts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 10:38:22 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/10 13:13:28 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include <stdio.h>

void	gc_print_layers(t_collector *gc)
{
	t_refs	*tmp;
	int	nb_layers;
	int		i;

	if (!gc)
		return ;
	i = -1;
	nb_layers = (int)gc->nb_layers;
	printf(GREEN BOLD "GC_PRINT_LAYERS\n" RESET);
	while (++i < nb_layers)
	{
		printf(YELLOW "\t[%d]:\n" RESET, i);
		tmp = gc->ref_layers[i];
		if (tmp)
			while(tmp && tmp->reference)
				printf("\t\t- %p\n", tmp->reference), tmp = tmp->next;
	}
}


void	gc_print(t_collector *gc)
{
	if (!gc)
		return ;
	if (gc->ref_layers)
		gc_print_layers(gc);
}
