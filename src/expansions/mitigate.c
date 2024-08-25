/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mitigate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:18:35 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/25 17:25:54 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	separator_reversion(char to_revert)
{
	if (to_revert == *SQ_SEP)
		return ('\'');
	else if (to_revert == *DQ_SEP)
		return ('\"');
	else if (to_revert == *CMD_SEP)
		return ('`');
}

static void	revert_all_seps_from_sq(char *to_expand)
{
	int	i;

	if (!to_expand)
		return ;
	i = -1;
	while (to_expand[++i])
	{
		
	}
}

void	separator_mitigation(t_expand *exp, t_mshell *data)
{
	int	i;

	if (!exp || !data || !exp->to_expand)
		return ;
	
}

//TODO REMPLACER LES SEPARATEURS EN TROP SELON LES QUOTES
//		UTILISER UN BOOL POUR LES VARIABLES (1/2 = $ | 2/2 = *exp->to_expand)
//		LE RESTE DES SEPARATEURS EST SYMETRIQUE DANS LA NOUVELLE CHAINE