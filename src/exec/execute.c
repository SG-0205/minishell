/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:24:41 by estegana          #+#    #+#             */
/*   Updated: 2024/10/09 09:57:13 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_execute(t_cmd *list)
{
	if (execve(list->args[0], list->args, list->env) == -1)
	{
		perror("EXECVE:");
		exit(errno);
	}
	return (0);
}
