/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:13:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/20 15:47:05 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_expand	*new_expansion(char *str, t_mshell *data)
{
	t_expand	*new;
	int			i;

	if (!str)
		return (NULL);
	i = -1;
	new = (t_expand *)gc_malloc(data->gc, sizeof(t_expand), 0);
	if (!new)
		return (NULL);
	new->to_expand = gc_strdup(str, data->gc, 0);
	if (!new->to_expand)
		return (NULL);
	count_quotes(new);
	while (MANAGED_QUOTES[++i] && i < 2)
		place_separator(new, MANAGED_QUOTES[i]);
	return (new);
}

static t_bool	check_pipe(char *pipe_start)
{
	int	i;

	i = -1;
	while (pipe_start[++i])
		if (ft_isalnum(pipe_start[i]) != 0)
			return (TRUE);
	return (FALSE);
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
		if (raw_input[i] == '|')
			if (check_pipe(&raw_input[i]) == TRUE)
				pipe_count ++;
	i = -1;
	while (raw_input[++i])
		if (raw_input[i] == '`')
			sub_count ++;
	if (sub_count % 2 != 0)
		sub_count --;
	return (pipe_count + (sub_count / 2));
}

// char	***make_cmds(char *raw_input, t_mshell *data)
// {
// 	char	***cmds;

// 	cmds = (char ***)gc_malloc(data->gc, (sizeof(char **)
// 				* (count_cmds(raw_input) + 1)), 0);
// 	if (!cmds)
// 		return (NULL);
// 	cmds[count_cmds(raw_input)] = NULL;
// }
