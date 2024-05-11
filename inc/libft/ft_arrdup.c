/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:43:31 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/03/19 15:35:07 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_arrdup(void **dst, void **src)
{
	int	i;

	if (!dst || !src)
		return ;
	i = -1;
	while (src[++i])
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
			return ;
	}
	dst[i] = NULL;
}
