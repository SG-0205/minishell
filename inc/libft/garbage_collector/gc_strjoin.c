/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:33:06 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/16 15:20:58 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

char	*gc_strjoin(const char *s1, char const *s2, t_collector *gc,
		size_t layer)
{
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	new_str = (char *)gc_malloc(gc, (sizeof(char) * (ft_strlen(s1)
					+ ft_strlen(s2) + 1)), layer);
	if (!new_str)
		return (NULL);
	ft_bzero(new_str, (ft_strlen(s1) + ft_strlen(s2) + 1));
	ft_strcat(new_str, s1);
	ft_strcat(new_str, s2);
	return (new_str);
}
