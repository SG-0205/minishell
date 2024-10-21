/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_utils4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:58:59 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/09 02:42:03 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	read_redirection(char *red_start, t_mshell *data, char *new, char *str)
{
	int	i;

	if (!red_start || !data || !new)
		return (0);
	i = cpy_redir(red_start, new);
	while (red_start[i])
	{
		if (ft_cisar(red_start[i], "<>| \t") == 1
			&& (is_quoted_by('\'', &red_start[i], str) == FALSE
				&& is_quoted_by('\"', &red_start[i], str) == FALSE))
			break ;
		new = ft_strncat(new, &red_start[i], 1);
		i ++;
	}
	new = ft_strncat(new, RED_SEP, 1);
	return (i - 1);
}

char	*mark_redirections(char *input, t_mshell *data)
{
	int		i;
	char	*new;

	if (!input)
		return (NULL);
	i = -1;
	new = gc_strnew(ft_strlen(input) * ft_strlen(input), data->gc, 1);
	while (input[++i])
	{
		if (ft_cisar(input[i], "<>") == 1
			&& is_quoted_by('\'', &input[i], input) == FALSE
			&& is_quoted_by('\"', &input[i], input) == FALSE)
			i += read_redirection(&input[i], data, new, input);
		else
			new = ft_strncat(new, &input[i], 1);
	}
	return (new);
}

char	*args_separation(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
	{
		if ((ft_cisar(input[i], ARG_SEPARATORS) == 1 && is_quoted_by('\'',
					&input[i], input) == FALSE && is_quoted_by('\"', &input[i],
					input) == FALSE))
			input[i] = *ARG_SEP;
	}
	return (input);
}

int	ft_lentillptr(char *c, char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (&str[i] == c)
			return (i);
	return (ft_lentillc(str, *c));
}

t_bool	check_2_quotes(char *ptr_in_str, char *str)
{
	if (is_quoted_by('\'', ptr_in_str, str) == TRUE
		&& is_quoted_by('\"', ptr_in_str, str) == TRUE)
		return (TRUE);
	else if (is_quoted_by('\'', ptr_in_str, str) == FALSE
		&& is_quoted_by('\"', ptr_in_str, str) == FALSE)
		return (FALSE);
	else
		return (ERROR);
}
