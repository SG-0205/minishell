/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:55 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 10:45:08 by sgoldenb         ###   ########.fr       */
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
	size_t	nb_layers;
	size_t	nb_refs;
}			t_collector;

t_collector	*gc_init(int nb_layers);
void		*gc_malloc(t_collector *gc, size_t size, size_t layer);
int			gc_flush(t_collector *gc);
int			gc_flush_layer(t_collector *gc, size_t layer);
int			gc_add_ref(t_collector *gc, void *ref, size_t layer);
t_refs		*search_ref(void *ref, t_refs *refs);
t_refs		*gc_search(void *ref, t_collector *gc);
int			free_ref(t_refs *ref);

//DEBUG
void		gc_print_layers(t_collector *gc);

#endif