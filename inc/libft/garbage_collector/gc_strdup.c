/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 22:25:52 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/18 22:36:15 by sgoldenb         ###   ########.fr       */
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
