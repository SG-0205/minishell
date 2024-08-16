/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:35:11 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/16 16:36:47 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

char	*gc_strnew(size_t size, t_collector *gc, size_t layer)
{
	char	*strnew;

	strnew = (char *)gc_malloc(gc, (size + 1) * sizeof(char), layer);
	if (!strnew)
		return (NULL);
	ft_bzero(strnew, size + 1);
	return (strnew);
}
