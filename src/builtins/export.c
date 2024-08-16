/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:50:05 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/16 17:27:28 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	check_args(char *args)
{
	int	i;

	i = -1;
	if (ft_isdigit(args[0]) != 0)
		return (FALSE);
	while (args[++i])
		if (ft_cisar(args[i], EXPORT_FORBIDDEN_CHARS) == 1)
			return (FALSE);
	return (TRUE);
}

int	export(t_mshell *data, char *args)
{
	if (!data)
		return (NODATA);
	if (!args || check_args(args) == FALSE)
		return (EXP_ERROR);
	//TODO = PARAM EXPAN
}