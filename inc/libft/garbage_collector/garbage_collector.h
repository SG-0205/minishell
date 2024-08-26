/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 15:23:55 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 16:44:56 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H
# include "../libft.h"

# define PREV 0
# define NEXT 1

typedef struct data
{
	t_list	**ref_layers;
	size_t	nb_layers;
	size_t	nb_fcts;
	size_t	nb_refs;
}			t_collector;

t_collector	*gc_init(int nb_layers);
void		*gc_malloc(t_collector *gc, size_t size, size_t layer);
int			gc_flush(t_collector *gc);
void		gc_flush_layer(t_collector *gc, size_t layer);
int			gc_add_ref(t_collector *gc, void *ref, size_t layer);
t_list		*search_ref(void *ref, t_list *refs);
t_list		*gc_search(void *ref, t_collector *gc);
int			free_ref(t_list *ref);
t_list		*gc_get_last(t_list *start);
int			gc_free(t_collector *gc, void *ptr);
t_list		*get_side_ref(t_list *target, t_list *lst_start, int side);
int			gc_switch_layer(t_collector *gc, void *ref, size_t layer);
void		*gc_replace_ref(t_collector *gc, void *new_ref, void *old_ref);
t_list		*get_layer(t_collector *gc, void *ref);
char		**gc_split(char const *s, char c, t_collector *gc, size_t layer);
char		*gc_strtrim(char const *s, char const *set, t_collector *gc,
				size_t layer);
char		*gc_strjoin(const char *s1, char const *s2, t_collector *gc,
				size_t layer);
char		*gc_strnew(size_t size, t_collector *gc, size_t layer);
char		*gc_strdup(char *str, t_collector *gc, size_t layer);
char		*gc_itoa(int n, t_collector *gc, size_t layer);
char		*gc_strjoinsep(const char *s1, char const *s2,
				char sep, t_collector *gc);
// DEBUG
void		gc_print_layers(t_collector *gc);
void		gc_print(t_collector *gc);

#endif