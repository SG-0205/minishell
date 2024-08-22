/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 22:05:39 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/22 19:50:11 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	check_pipe(char *pipe_start)
{
	int	i;

	i = -1;
	while (pipe_start[++i])
		if (ft_isalnum(pipe_start[i]) != 0)
			return (TRUE);
	return (FALSE);
}

static t_bool	check_parenthesis(char *start)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!start || *start != '$')
		return (FALSE);
	while (start[++i] && count < 2)
	{
		if (count % 2 == 0 && start[i] == '(')
			count ++;
		else if (count % 2 != 0 && start[i] == ')')
			count ++;
	}
	if (count == 2)
		return (TRUE);
	else
		return (FALSE);
}

static int	check_dollar_fct(char *raw_input)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	if (!raw_input)
		return (count);
	while (raw_input[++i])
		if (raw_input[i] == '$' && check_parenthesis(&raw_input[i]) == TRUE)
			count ++;
	return (count);
}

int	count_cmds(char *raw_input)
{
	int	i;
	int	pipe_count;
	int	sub_count;

	i = -1;
	pipe_count = 1;
	sub_count = 0;
	while (raw_input[++i])
		if (raw_input[i] == '|' && check_pipe(&raw_input[i]) == TRUE)
			pipe_count ++;
	i = -1;
	while (raw_input[++i])
		if (raw_input[i] == '`')
			sub_count ++;
	if (sub_count % 2 != 0)
		sub_count --;
	return (pipe_count + (sub_count / 2) + check_dollar_fct(raw_input));
}
