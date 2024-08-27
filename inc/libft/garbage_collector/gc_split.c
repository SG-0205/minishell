/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:11:23 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/27 12:06:41 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

static void	ft_checkalloc(char **array, size_t len)
{
	while (len --)
		free(array[len]);
	free(array);
}

static void	ft_initarray(char **array, const char *s, char c, t_collector *gc)
{
	t_bool	is_init;
	size_t	i;
	size_t	j;
	size_t	nb_wrds;

	is_init = 0;
	j = 0;
	i = 0;
	nb_wrds = ft_countwords(s, c);
	while (s[j] && i < nb_wrds)
	{
		if (s[j] == c)
			is_init = FALSE;
		else if (s[j] != c && is_init == FALSE)
		{
			array[i] = (char *)gc_malloc(gc,
					((ft_lentillc(&(s[j]), c) + 1) * sizeof(char)), 0);
			if (!array[i])
				return (ft_checkalloc(array, i));
			ft_bzero((void *)array[i], ft_lentillc(&(s[j]), c) + 1);
			is_init = TRUE;
			i ++;
		}
		j ++;
	}
}

static void	ft_fillarray(char **array, const char *s, char c)
{
	t_bool	start_cpy;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	start_cpy = 0;
	while (s[j])
	{
		if (s[j] == c)
			start_cpy = FALSE;
		else if (s[j] != c && start_cpy == FALSE)
		{
			array[i] = ft_strncpy(array[i], &(s[j]), ft_lentillc(&(s[j]), c));
			start_cpy = TRUE;
			i ++;
		}
		j ++;
	}
}

/*
Adaptation de split pour le collecteur, ne fonctionne que sur la couche 0 (NKO)
*/
char	**gc_split(char const *s, char c, t_collector *gc, size_t layer)
{
	char	**array;

	if (!s)
		return (NULL);
	array = (char **)gc_malloc(gc, (ft_countwords(s, c) + 1)
			* sizeof(char *), layer);
	if (!array)
		return (NULL);
	ft_initarray(array, s, c, gc);
	ft_fillarray(array, s, c);
	array[ft_countwords(s, c)] = NULL;
	return (array);
}
