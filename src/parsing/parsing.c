/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 22:05:39 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/27 13:31:33 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	check_pipe(char *pipe_start)
{
	int	i;

	i = -1;
	while (pipe_start[++i])
		if (ft_isprint(pipe_start[i]) != 0)
			return (TRUE);
	return (FALSE);
}

int	count_cmds(char *raw_input)
{
	int	i;
	int	pipe_count;

	i = -1;
	pipe_count = 1;
	while (raw_input[++i])
		if (raw_input[i] == '|' && check_pipe(&raw_input[i]) == TRUE
			&& is_quoted_by('\'', &raw_input[i], raw_input) == FALSE
			&& is_quoted_by('\"', &raw_input[i], raw_input) == FALSE)
			pipe_count ++;
	return (pipe_count);
}
