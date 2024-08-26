/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strjoinsep.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:42:14 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 16:45:01 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

char	*gc_strjoinsep(const char *s1, char const *s2, char sep,
	t_collector *gc)
{
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	new_str = gc_strnew((ft_strlen(s1) + ft_strlen(s2)) + 1, gc, 0);
	if (!new_str)
		return (NULL);
	ft_strcat(new_str, s1);
	new_str[ft_strlen(new_str)] = sep;
	ft_strcat(new_str, s2);
	return (new_str);
}
