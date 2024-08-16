/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrfind.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:26:01 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/16 09:47:24 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_arrfind(char **array, char *to_find)
{
	int	i;

	if (!array || !*array || !to_find)
		return (NULL);
	i = -1;
	while (array[++i])
		if (!ft_strncmp(array[i], to_find, ft_strlen(to_find)))
			return (array[i]);
	return (NULL);
}
