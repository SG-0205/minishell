/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:11:58 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/26 16:14:44 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	append_fd(char *path)
{
	int	fd;

	if (!path)
		return (-2);
	fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0600);
	if (fd < 0)
		return (fd);
	return (0);
}