/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:49:00 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/25 13:34:15 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	count_quotes(t_expand *str)
{
	int	i;

	i = -1;
	if (!str || !str->to_expand)
		return ;
	while (str->to_expand[++i])
	{
		if (str->to_expand[i] == '\'')
			str->sq_count += 1;
		if (str->to_expand[i] == '\"')
			str->dq_count += 1;
	}
}

int	get_limiter(t_expand *str, char to_replace)
{
	if (!str)
		return (-1);
	if (to_replace == '\'' && str->sq_count % 2 != 0)
		return (1);
	else if (to_replace == '\'' && str->sq_count % 2 == 0)
		return (0);
	if (to_replace == '\"' && str->dq_count % 2 != 0)
		return (1);
	else if (to_replace == '\"' && str->dq_count % 2 == 0)
		return (0);
	else
		return (0);
}

char	get_separator(char to_replace)
{
	if (to_replace == '\'')
		return (*SQ_SEP);
	else if (to_replace == '\"')
		return (*DQ_SEP);
	else
		return (0);
}

char	get_count(t_expand *str, char to_replace)
{
	if (!str)
		return (0);
	if (to_replace == '\'')
		return (str->sq_count);
	else if (to_replace == '\"')
		return (str->dq_count);
	else
		return (0);
}

void	place_separator(t_expand *str, char to_replace)
{
	int		i;
	int		limit;
	char	sep;
	int		count;

	if (!str || !str->to_expand)
		return ;
	sep = get_separator(to_replace);
	limit = get_limiter(str, to_replace);
	count = get_count(str, to_replace);
	i = -1;
	while (str->to_expand[++i] && count > limit)
	{
		if (str->to_expand[i] == to_replace)
		{
			str->to_expand[i] = sep;
			count --;
		}
	}
}
