/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:55 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/11 16:30:32 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H
# include "../libft.h"

typedef struct s_refs t_refs;

typedef struct s_refs
{
	void	*reference;
	t_refs	*next;
}			t_refs;

typedef struct data
{
	t_refs	**ref_layers;
	void	**additional_fcts;
	int		nb_layers;
	int		nb_refs;
}			t_collector;

t_collector	*gc_init(int nb_layers);
int			add_ref(t_collector *gc, void *ref, int layer);

#endif