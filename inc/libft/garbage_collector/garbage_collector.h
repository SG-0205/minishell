/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:55 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 17:06:48 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H
# include "../libft.h"

typedef struct s_refs	t_refs;
typedef void			*(*t_fct_ptr)(void **);

typedef struct s_refs
{
	void				*reference;
	t_refs				*next;
}						t_refs;

typedef struct data
{
	t_refs				**ref_layers;
	t_fct_ptr			*additional_fcts;
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
int						gc_free(t_collector *gc, void *ptr);
int						gc_init_fcts(t_collector *gc, size_t nb_fcts);
int						gc_add_fct(t_collector *gc, void *(*f)(void **));
int						gc_do_fct(t_collector *gc, size_t fct_id, void *ref);
int						gc_flush_fcts(t_collector *gc);
int						gc_fct_on_layer(t_collector *gc, size_t layer,
							size_t fct_id);
int						gc_fct_all_refs(t_collector *gc, size_t fct_id);
// DEBUG
void					gc_print_layers(t_collector *gc);
void					gc_print_fcts(t_collector *gc);
void					gc_print(t_collector *gc);

#endif