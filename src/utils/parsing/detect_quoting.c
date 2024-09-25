/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_quoting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:35:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/03 15:44:55 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_bool	is_quoted_by(char quote_type, char *ptr_in_str, char *str)
{
	int		i;
	t_bool	quoted;

	if (!ptr_in_str || !str)
		return (FALSE);
	i = -1;
	quoted = FALSE;
	while (str[++i])
	{
		if (str[i] == quote_type && quoted == FALSE)
			quoted = TRUE;
		else if (str[i] == quote_type && quoted == TRUE)
			quoted = FALSE;
		else if (ptr_in_str == &str[i] && quoted == TRUE)
			return (TRUE);
	}
	return (FALSE);
}

// static void	setup_tabs(char *sq[2], char *dq[2], t_bool quoted[3])
// {
// 	if (!sq || !dq || !quoted)
// 		return ;
// 	sq[0] = NULL;
// 	sq[1] = NULL;
// 	dq[0] = NULL;
// 	dq[1] = NULL;
// 	quoted[0] = FALSE;
// 	quoted[1] = FALSE;
// 	quoted[2] = TRUE;
// }

// static void	locate_quotes(char *sq[2], char *dq[2], t_bool quoted[3], char *c)
// {
// 	if (*c == '\'' && quoted[0] == FALSE)
// 	{
// 		quoted[0] = TRUE;
// 		sq[0] = c;
// 	}
// 	else if (*c == '\'' && quoted[0] == TRUE)
// 	{
// 		sq[1] = c;
// 		quoted[0] = FALSE;
// 	}
// 	else if (*c == '\"' && quoted[1] == FALSE)
// 	{
// 		quoted[1] = TRUE;
// 		dq[0] = c;
// 	}
// 	else if (*c == '\"' && quoted[1] == TRUE)
// 	{
// 		dq[1] = c;
// 		quoted[1] = FALSE;
// 	}
// }

// static char	compare_quote_positions(char *sq[2], char *dq[2])
// {
// 	if (sq[0] < dq[0])
// 		return (*sq[0]);
// 	else if (dq[0] < sq[0])
// 		return (*dq[0]);
// 	else
// 		return (0);
// }

// char	first_quoting(char *ptr_in_str, char *str)
// {
// 	char	*sq_loc[2];
// 	char	*dq_loc[2];
// 	t_bool	quoted[3];
// 	int		i;

// 	if (!str || !ptr_in_str)
// 		return (FALSE);
// 	i = -1;
// 	if (!(is_quoted_by('\"', ptr_in_str, str) == TRUE && is_quoted_by('\'',
// 			ptr_in_str, str) == TRUE))
// 		return (ret_quoting(ptr_in_str, str));
// 	setup_tabs(sq_loc, dq_loc, quoted);
// 	while (str[++i])
// 	{
// 		if (quoted[2] == FALSE && sq_loc[1] && dq_loc[1])
// 			break ;
// 		else if (&str[i] == ptr_in_str && quoted[2] == TRUE)
// 			quoted[2] = FALSE;
// 		else if (str[i] == '\'' || str[i] == '\"')
// 			locate_quotes(sq_loc, dq_loc, quoted, &str[i]);
// 	}
// 	return (compare_quote_positions(sq_loc, dq_loc));
// }

// UTILISER MATHS SUR LES POINTEURS POUR VERIFIER LA PREMIERE QUOTES OUVERTE
//MAUVAISE APPROCHE GLOBALE! FUCK!!!


char	first_quoting(char *ptr_in_str, char *str)
{
	int		i;
	char	*dq_first;
	char	*sq_first;

	if (!ptr_in_str || !str)
		return (0);
	i = -1;
	while (str[++i])
	{
		if (&str[i] == ptr_in_str)
			break ;
		else if (str[i] == '\'' && is_closed(&str[i], str) == TRUE)
			sq_first = &str[i];
		else if (str[i] == '\"' && is_closed(&str[i], str) == TRUE)
			dq_first = &str[i];
	}
	if (dq_first < sq_first)
		return (*dq_first);
	else
		return (*sq_first);
}
