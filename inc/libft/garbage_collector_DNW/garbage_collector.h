/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:55 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/10 13:15:06 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H
# include "../libft.h"

# define PREV 0
# define NEXT 1

typedef struct s_refs	t_refs;

typedef struct s_refs
{
	void				*reference;
	t_refs				*next;
}						t_refs;

typedef struct data
{
	t_refs				**ref_layers;
	size_t				nb_layers;
	size_t				nb_fcts;
	size_t				nb_refs;
}						t_collector;

t_collector				*gc_init(int nb_layers);
void					*gc_malloc(t_collector *gc, size_t size, size_t layer);
int						gc_flush(t_collector *gc);
int						gc_flush_layer(t_collector *gc, size_t layer);
int						gc_add_ref(t_collector *gc, void *ref, size_t layer);
t_refs					*search_ref(void *ref, t_refs *refs);
t_refs					*gc_search(void *ref, t_collector *gc);
int						free_ref(t_refs *ref);
t_refs					*gc_get_last(t_refs *start);
int						gc_free(t_collector *gc, void *ptr);
t_refs					*get_side_ref(t_refs *target,
							t_refs *lst_start, int side);
int						gc_switch_layer(t_collector *gc,
							void *ref, size_t layer);
void					*gc_replace_ref(t_collector *gc,
							void *new_ref, void *old_ref);
t_refs					*get_layer(t_collector *gc, void *ref);
// DEBUG
void					gc_print_layers(t_collector *gc);
void					gc_print(t_collector *gc);

#endif