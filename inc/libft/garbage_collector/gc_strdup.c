/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 22:25:52 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/07 19:43:01 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

char	*gc_strdup(char *str, t_collector *gc, size_t layer)
{
	char	*new;

	if (!str || !gc)
		return (NULL);
	new = (char *)gc_malloc(gc, (sizeof(char) * (ft_strlen(str) + 1)), layer);
	if (!new)
		return (NULL);
	new = ft_strcpy(new, str);
	return (new);
}

char	*gc_strndup(char *str, int n, t_collector *gc, size_t layer)
{
	char	*new;

	if (!str || !gc)
		return (NULL);
	new = gc_strnew(ft_strlen(str), gc, layer);
	if (!new)
		return (NULL);
	new = ft_strncat(new, str, n);
	return (new);
}
