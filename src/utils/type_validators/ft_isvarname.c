/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isvarname.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 12:05:27 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/25 12:20:58 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_bool	ft_isvarname(char var_char)
{
	if (var_char != '_' && ft_isalnum(var_char) == 0)
		return (FALSE);
	return (TRUE);
}

t_bool	validate_var(char *var_start)
{
	int	i;

	i = 0;
	if (var_start[i] != '$')
		return (FALSE);
	i++;
	if (ft_cisar(var_start[i], EXPORT_FORBIDDEN_CHARS) != 0
		|| ft_cisar(var_start[i], "123456789 ") != 0
		|| var_start[i] == '\0')
		return (FALSE);
	return (TRUE);
}
