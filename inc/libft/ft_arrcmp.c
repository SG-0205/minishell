/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:35:54 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/03/28 13:02:40 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_arrcmp(char **array, char *str)
{
	int	arrsize;

	if (!array || !*array || !str)
		return (NULL);
	arrsize = ft_arrlen((void **)array);
	while (--arrsize > -1)
		if (ft_strcmp(array[arrsize], str) == 0)
			return (array[arrsize]);
	return (NULL);
}
