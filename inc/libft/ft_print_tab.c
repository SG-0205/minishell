/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_tab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:49:22 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/25 15:02:54 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_tab(char **tab)
{
	int	size;
	int	i;

	if (!tab || !*tab)
		return ;
	i = -1;
	size = ft_arrlen((void **)tab);
	while (++i < size)
	{
		ft_putstr(tab[i]);
		ft_putchar('\n');
	}
}
