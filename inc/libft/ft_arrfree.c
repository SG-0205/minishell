/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 17:15:02 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 11:05:56 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_arrfree(void **array)
{
	int	size;

	if (!array || !*array)
		return ;
	size = ft_arrlen(array);
	while (-- size > -1)
		free(array[size]);
}
