/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:51:26 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/16 09:53:27 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

char	*gc_strtrim(char const *s, char const *set, t_collector *gc,
	size_t layer)
{
	size_t	start;
	size_t	end;
	char	*new_s;

	if (!s || !set)
		return (NULL);
	new_s = NULL;
	if (s != 0 || set != 0)
	{
		start = 0;
		end = ft_strlen(s);
		while (s[start] && ft_strchr(set, s[start]))
			start++;
		while (s[end - 1] && ft_strchr(set, s[end - 1]) && end > start)
			end--;
		new_s = (char *)gc_malloc(gc, sizeof(char) * (end - start + 1), layer);
		if (!new_s)
			return (NULL);
		if (new_s)
			ft_strlcpy(new_s, &s[start], end - start + 1);
	}
	return (new_s);
}
