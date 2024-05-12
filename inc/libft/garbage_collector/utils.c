/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:56:56 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/05/12 19:02:00 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"

t_refs	*get_bflast(t_refs *refs)
{
	t_refs	*tmp;

	if (!refs)
		return (NULL);
	tmp = refs;
	while (tmp->next)
	{
		if (tmp->next->next == NULL)
			return (tmp);
		tmp = tmp->next;
	}
	return (tmp);
}
