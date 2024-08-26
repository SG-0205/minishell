/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mitigate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 16:18:35 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 13:48:12 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	separator_reversion(char to_revert)
{
	if (to_revert == *SQ_SEP)
		return ('\'');
	else if (to_revert == *CMD_SEP)
		return ('\"');
	else if (to_revert == *CMD_SEP)
		return ('`');
	else
		return (0);
}

static void	handle_sq_quoted_vars(t_expand *exp)
{
	int		i;
	t_bool	var_start;

	if (!exp || !exp->expanded)
		return ;
	i = -1;
	var_start = FALSE;
	while (exp->expanded[++i])
	{
		if (var_start == FALSE
			&& is_quoted(*SQ_SEP, &exp->expanded[i], exp->expanded) == TRUE
			&& exp->expanded[i] == *VAR_SEP)
		{
			exp->expanded[i] = '$';
			var_start = TRUE;
		}
		else if (var_start == TRUE
			&& is_quoted(*SQ_SEP, &exp->expanded[i], exp->expanded) == TRUE
			&& exp->expanded[i] == *VAR_SEP)
			str_shrink(&exp->expanded[i]);
	}
}

static void	revert_all_seps_from_sq(t_expand *exp)
{
	int	i;

	if (!exp || !exp->to_expand || !exp->expanded)
		return ;
	i = -1;
	while (exp->expanded[++i] && exp->expanded[i + 1])
	{
		if (is_sep(exp->expanded[i]) == TRUE
			&& is_quoted(*SQ_SEP, &exp->expanded[i], exp->expanded) == TRUE
			&& exp->expanded[i] == *DQ_SEP)
			exp->expanded[i] = separator_reversion(*DQ_SEP);
		else if (is_sep(exp->expanded[i]) == TRUE
			&& is_quoted(*SQ_SEP, &exp->expanded[i], exp->expanded) == TRUE
			&& exp->expanded[i] == *SQ_SEP)
			exp->expanded[i] = separator_reversion(*SQ_SEP);
		else if (is_sep(exp->expanded[i]) == TRUE
			&& is_quoted(*SQ_SEP, &exp->expanded[i], exp->expanded) == TRUE
			&& exp->expanded[i] == *CMD_SEP)
			exp->expanded[i] = separator_reversion(*CMD_SEP);
	}
	handle_sq_quoted_vars(exp);
}

void	separator_mitigation(t_expand *exp)
{
	if (!exp || !exp->to_expand || !exp->expanded)
		return ;
	printf(CYAN "B4 MITIGATION:\t%s\n" RESET, exp->expanded);
	if (exp->sq_count > 1)
		revert_all_seps_from_sq(exp);
	printf(CYAN "B4 MITIGATION:\t%s\n" RESET, exp->expanded);
}

//TODO REMPLACER LES SEPARATEURS EN TROP SELON LES QUOTES
//		UTILISER UN BOOL POUR LES VARIABLES (1/2 = $ | 2/2 = *exp->to_expand)
//		LE RESTE DES SEPARATEURS EST SYMETRIQUE DANS LA NOUVELLE CHAINE