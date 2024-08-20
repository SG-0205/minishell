/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 13:20:27 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/20 15:49:03 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

static int	len(long int nb)
{
	int		len;

	len = 0;
	if (nb < 0)
	{
		nb = nb * -1;
		len++;
	}
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

char	*gc_itoa(int n, t_collector *gc, size_t layer)
{
	char		*str;
	int			i;
	long int	nb;

	if (!gc)
		return (NULL);
	nb = n;
	i = len(nb);
	str = gc_strnew(i, gc, layer);
	if (!str)
		return (NULL);
	if (nb == 0)
		str[0] = 48;
	if (nb < 0)
	{
		str[0] = '-';
		nb = nb * -1;
	}
	while (nb > 0 && i-- >= 0)
	{
		str[i] = 48 + (nb % 10);
		nb = nb / 10;
	}
	return (str);
}
