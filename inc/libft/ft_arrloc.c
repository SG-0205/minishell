/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:49:27 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/03/25 14:54:20 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_arrloc(char **array, char *to_search)
{
	int	location;

	if (!array || !*array || !to_search)
		return (-1);
	location = ft_arrlen((void **)array);
	while (--location > -1)
		if (ft_strcmp(array[location], to_search) == 0)
			return (location);
	return (-1);
}
