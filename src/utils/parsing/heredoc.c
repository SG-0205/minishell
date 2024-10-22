/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:45:11 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/22 11:17:05 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_bool	check_bad_limiter(char *limiter, t_mshell *data)
{
	if (*limiter == 0)
	{
		syntax_error(limiter, data);
		return (FALSE);
	}
	g_sig_trace = TRUE;
	return (TRUE);
}

void	*bad_eof_hd(char *limiter, int l_count, t_mshell *data)
{
	if (!data)
		return (NULL);
	bad_eof(limiter, l_count, data);
	data->bad_hd = TRUE;
	return (NULL);
}

char	*rm_limiter_quoting(char *limiter)
{
	int	i;

	if (!limiter)
		return (NULL);
	i = -1;
	while (limiter[++i])
		if ((limiter[i] == '\'' || limiter[i] == '\"') && is_quoted_by('\'',
				&limiter[i], limiter) == FALSE && is_quoted_by('\"',
				&limiter[i], limiter) == FALSE)
			str_shrink(&limiter[i]);
	return (limiter);
}

t_hd_l_type	get_hd_output_type(char *limiter)
{
	t_hd_l_type	type;
	int			i;

	limiter = quote_closure_control(limiter);
	if (*limiter == '-')
		type = UNTAB;
	else
		type = NORMAL;
	i = -1;
	while (limiter[++i])
	{
		if (limiter[i] == '\'' && type == UNTAB)
			type = UNTAB_SQ;
		else if (limiter[i] == '\'' && type == NORMAL)
			type = SQ;
	}
	limiter = rm_limiter_quoting(limiter);
	limiter = revert_unclosed(limiter);
	return (type);
}

char	*rm_tabulations(char *final)
{
	int	i;

	if (!final)
		return (NULL);
	i = -1;
	while (final[++i])
		if (final[i] == '\t')
			str_shrink(&final[i]);
	return (final);
}
