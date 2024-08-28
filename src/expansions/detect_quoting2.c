/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_quoting2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:11:08 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/28 23:09:17 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	ret_quoting(char *ptr_in_str, char *str)
{
	if (is_quoted_by('\"', ptr_in_str, str) == TRUE)
		return ('\"');
	else if (is_quoted_by('\'', ptr_in_str, str) == TRUE)
		return ('\'');
	else
		return (0);
}

t_bool	is_quoted(char *ptr_in_str, char *str, t_bool check_both)
{
	if (!ptr_in_str || !str || ptr_in_str == str)
		return (FALSE);
	if (check_both == TRUE)
	{
		if (is_quoted_by('\'', ptr_in_str, str) == TRUE
			&& is_quoted_by('\"', ptr_in_str, str) == TRUE)
			return (TRUE);
		else
			return (FALSE);
	}
	else
	{
		if (is_quoted_by('\'', ptr_in_str, str) == TRUE
			|| is_quoted_by('\"', ptr_in_str, str) == TRUE)
			return (TRUE);
	}
	return (FALSE);
}
