/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinsep.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 17:18:50 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/03/19 17:20:03 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// #include <stdio.h>

char	*ft_strjoinsep(const char *s1, char const *s2, char sep)
{
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	new_str = ft_strnew((ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!new_str)
		return (NULL);
	ft_strcat(new_str, s1);
	new_str[ft_strlen(new_str)] = sep;
	ft_strcat(new_str, s2);
	return (new_str);
}
