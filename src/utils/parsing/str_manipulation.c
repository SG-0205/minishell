/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_manipulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:35:23 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 13:37:30 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	str_shrink(char *str)
{
	int	i;

	i = -1;
	while (str[++i + 1])
		str[i] = str[i + 1];
	str[i] = 0;
}