/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strblen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:09:49 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/26 15:16:15 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strblen(char *str, int (*validation)(char))
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[++i] && validation(str[i]) == 0)
		i ++;
	return (i);
}
