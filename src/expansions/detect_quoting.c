/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_quoting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:35:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 13:50:04 by sgoldenb         ###   ########.fr       */
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
		if (ptr_in_str == &str[i] && quoted == TRUE)
			return (TRUE);
		else if (str[i] == quote_type && quoted == FALSE)
			quoted = TRUE;
		else if (str[i] == quote_type && quoted == TRUE)
			quoted = FALSE;
	}
	return (FALSE);
}
