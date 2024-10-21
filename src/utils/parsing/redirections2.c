/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirctions2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:40:15 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/08 13:46:54 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	is_not_operator(char c)
{
	if (ft_cisar(c, "<>|") == 1)
		return (0);
	else
		return (1);
}

t_bool	has_redir(char *input)
{
	int	i;

	if (!input)
		return (FALSE);
	i = -1;
	while (input[++i])
	{
		if (ft_cisar(input[i], "<>") == 1 && is_quoted_by('\'', &input[i],
				input) == FALSE && is_quoted_by('\"', &input[i],
				input) == FALSE)
			return (TRUE);
	}
	return (FALSE);
}

char	*rm_redirections(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
	{
		if (input[i] == *RED_SEP)
		{
			input[i] = ' ';
			while (input[i++] && input[i] != *RED_SEP)
				input[i] = ' ';
			input[i] = ' ';
		}
	}
	return (input);
}

t_bool	is_single_cmd(char **input)
{
	int	i;
	int	j;

	if (!input)
		return (ERROR);
	i = -1;
	while (input[++i])
	{
		j = -1;
		while (input[i][++j])
			if (input[i][j] == '|' && is_quoted_by('\'', &input[i][j],
					input[i]) == FALSE && is_quoted_by('\"', &input[i][j],
					input[i]) == FALSE)
				return (FALSE);
	}
	return (TRUE);
}

int	r_id_by_reference(char **r_token, char **ref)
{
	int		id;
	t_bool	passed;
	int		i;
	int		j;

	i = -1;
	id = 0;
	passed = FALSE;
	while (ref[++i])
	{
		if (&ref[i] == r_token)
			passed = TRUE;
		j = -1;
		while (ref[i][++j])
		{
			if (ref[i][j] == '|' && check_2_quotes(&ref[i][j], ref[i]) == FALSE)
			{
				id++;
				if (passed == TRUE)
					return (id - 1);
			}
		}
	}
	return (id);
}
