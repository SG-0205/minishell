/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_quoting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:35:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/27 16:28:22 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_bool	is_quoted(char quote_type, char *ptr_in_str, char *str)
{
	int		i;
	t_bool	quoted;

	if (!ptr_in_str || !str || ptr_in_str == str)
		return (FALSE);
	i = -1;
	quoted = FALSE;
	while (str[++i])
	{
		printf("PTR / &STR[%d]:\t%p : %p\n", i, ptr_in_str, &str[i]);
		if (str[i] == quote_type && quoted == FALSE)
			quoted = TRUE;
		else if (str[i] == quote_type && quoted == TRUE)
			quoted = FALSE;
		else if (ptr_in_str == &str[i] && quoted == TRUE)
			return (TRUE);
	}
	return (FALSE);
}

static void	setup_tabs(char **sq, char **dq, t_bool *quoted)
{
	if (!sq || !dq || !quoted)
		return ;
	sq[0] = NULL;
	sq[1] = NULL;
	dq[0] = NULL;
	dq[1] = NULL;
	quoted[0] = FALSE;
	quoted[1] = FALSE;
}


t_bool	first_quoting(char quote_type, char *ptr_in_str, char *str)
{
	char	*sq_loc[2];
	char	*dq_loc[2];
	t_bool	quoted[2];
	int		i;

	if (!str || !ptr_in_str || str == ptr_in_str)
		return (FALSE);
	i = -1;
	if (is_quoted(*DQ_SEP, ptr_in_str, str) == FALSE
		|| is_quoted(*SQ_SEP, ptr_in_str, str) == FALSE)
	return (FALSE);
	setup_tabs(sq_loc, dq_loc, quoted);
	while (str[++i])
	{
		if (str[i] == '\'' && quoted[0] == FALSE)
		{
			quoted[0] = TRUE;
			sq_loc[0] = &str[i];
		}
		else if (str[i] == '\'' && quoted[0] == TRUE)
		{
			quoted[]
		}
	}
}

//UTILISER MATHS SUR LES POINTEURS POUR VERIFIER LA PREMIERE QUOTES OUVERTE
