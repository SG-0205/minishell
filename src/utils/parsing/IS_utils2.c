/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 12:24:58 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/08 13:52:31 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	read_dq(char *dq_start, char *arg, char *new_arg, t_mshell *data)
{
	int	i;
	int	values_len;

	if (!dq_start || !arg || !new_arg || !data)
		return (0);
	i = 0;
	values_len = 0;
	while (dq_start[++i] && dq_start[i] != '\"')
	{
		if (dq_start[i] == '$' && validate_var(&dq_start[i]) == TRUE
			&& ((is_quoted_by('\"', &dq_start[i], dq_start) == TRUE
					&& is_quoted_by('\'', &dq_start[i], dq_start) == FALSE)
				|| (is_quoted_by('\'', &dq_start[i], arg) == TRUE
					&& first_quoting(&dq_start[i], arg) == '\"')))
		{
			values_len += try_insert_value(&dq_start[i], new_arg, data);
			i += ft_strlen(get_var_name(&dq_start[i], data));
		}
		else
			ft_strncat(new_arg, &dq_start[i], 1);
	}
	return (i);
}

int	read_sq(char *sq_start, char *arg, char *new_arg)
{
	int	i;

	if (!sq_start || !arg || !new_arg)
		return (0);
	i = 0;
	while (sq_start[++i] && sq_start[i] != '\'')
		ft_strncat(new_arg, &sq_start[i], 1);
	return (i);
}

t_bool	is_a_closing_quote(char *quote_ptr, char *str)
{
	int	i;
	int	count;

	if (!quote_ptr || !str)
		return (ERROR);
	i = -1;
	count = 0;
	while (str[++i] && &str[i] != quote_ptr)
		if (str[i] == *quote_ptr)
			count++;
	if (count % 2 == 0)
		return (FALSE);
	else
		return (TRUE);
}

t_bool	is_closed(char *quote_ptr, char *str)
{
	int	i;

	if (!quote_ptr || !str)
		return (FALSE);
	i = 0;
	while (quote_ptr[++i])
		if (quote_ptr[i] == *quote_ptr)
			return (TRUE);
	return (FALSE);
}
