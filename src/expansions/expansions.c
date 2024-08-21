/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:13:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/21 13:32:22 by sgoldenb         ###   ########.fr       */
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
	new->cmd_exp_count = 0;
	new->dq_count = 0;
	new->sq_count = 0;
	new->var_count = 0;
	new->to_expand = gc_strdup(str, data->gc, 0);
	if (!new->to_expand)
		return (NULL);
	count_quotes(new);
	while (MANAGED_QUOTES[++i] && i < 2)
		place_separator(new, MANAGED_QUOTES[i]);
	if (mark_vars(new, data) == VARS_ERROR)
		return (NULL);
	return (new);
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
