/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_quoting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:35:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/08 13:54:33 by sgoldenb         ###   ########.fr       */
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

char	first_quoting(char *ptr_in_str, char *str)
{
	int		i;
	char	*dq_first;
	char	*sq_first;

	if (!ptr_in_str || !str)
		return (0);
	i = -1;
	dq_first = NULL;
	sq_first = NULL;
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
