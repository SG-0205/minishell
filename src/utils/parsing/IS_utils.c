/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:23:29 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/25 20:03:42 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char **expand_all_args(char **args, t_mshell *data)
{
	int	i;

	i = -1;
	while (args[++i])
		args[i] = read_quoting(args[i], data);
	return (args);
}

char	*quote_closure_control(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
	{
		if (input[i] == '\"'
			&& is_closed(&input[i], input) == FALSE
			&& is_a_closing_quote(&input[i], input) == FALSE
			&& input[i + 1])
			input[i] = *DQ_SEP;
		else if (input[i] == '\''
			&& is_closed(&input[i], input) == FALSE
			&& is_a_closing_quote(&input[i], input) == FALSE
			&& input[i + 1])
			input[i] = *SQ_SEP;
	}
	return (input);
}

char	*revert_unclosed(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
		if (input[i] == *DQ_SEP)
			input[i] = '\"';
		else if (input[i] == *SQ_SEP)
			input[i] = '\'';
	return (input);
}

//IF NON QUOTED && FIND << INSERT HERDEOC