/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:13:37 by estegana          #+#    #+#             */
/*   Updated: 2024/10/21 14:01:15 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_exec(t_cmd *list)
{
	int	i;

	i = -1;
	while (++i < cmd_list_size(&list))
		if (pipe(list->link->fds[i]) < 0)
			printf("DEAD");
		// else
		// 	printf("CREATED:[%d]:\tIN[%d] OUT[%d]\n", i, list->link->fds[i][1], list->link->fds[i][0]);
	ft_loop(list);
	return (0);
}
