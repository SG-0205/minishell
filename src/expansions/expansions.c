/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:13:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/27 14:59:32 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_expand	*new_empty_exp_object(t_mshell *data)
{
	t_expand	*new;

	new = (t_expand *)gc_malloc(data->gc, sizeof(t_expand), 1);
	if (!new)
		return (NULL);
	new->cmd_exp_count = 0;
	new->dq_count = 0;
	new->sq_count = 0;
	new->var_count = 0;
	new->to_expand = NULL;
	new->vars_to_insert = NULL;
	new->expanded = NULL;
	return (new);
}

t_expand	*new_expansion(char *str, t_mshell *data)
{
	t_expand	*new;
	int			i;

	if (!str)
		return (NULL);
	i = -1;
	new = new_empty_exp_object(data);
	if (!new)
		return (NULL);
	new->to_expand = gc_strdup(str, data->gc, 1);
	if (!new->to_expand)
		return (NULL);
	new->expanded = gc_strdup(new->to_expand, data->gc, 1);
	if (!new->to_expand)
		return (NULL);
	count_quotes(new);
	print_exp(new, "AFTER COUNT");
	while (MANAGED_QUOTES[++i] && i < 2)
		place_separator(new, MANAGED_QUOTES[i]);
	print_exp(new, "AFTER SEPS");
	if (mark_vars(new, data) == VARS_ERROR)
		return (NULL);
	print_exp(new, "AFTER VARS");
	separator_mitigation(new);
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
