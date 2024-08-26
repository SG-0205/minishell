/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:08:18 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 11:15:36 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_bool	is_sep(char c)
{
	if (c == *DQ_SEP || c == *SQ_SEP || c == *VAR_SEP || c == *CMD_SEP)
		return (TRUE);
	else
		return (FALSE);
}
