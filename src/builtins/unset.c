/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:49:17 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/07 20:22:50 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	cut_var_from_env(char *arg, t_mshell *data)
{
	t_envar	*search_attempt;
	t_envar	**side_vars;

	if (!arg || !data || !data->env)
		return ;
	search_attempt = search_var(&data->env, arg);
	if (!search_attempt)
		return ;
	else if (search_attempt->hidden == FALSE)
	{
		side_vars = get_side_vars(search_attempt, data);
		if (side_vars[0] && side_vars[1])
			side_vars[0]->next = side_vars[1];
		else if (!side_vars[0] && side_vars[1])
			data->env = side_vars[1];
		else if (side_vars[0] && !side_vars[1])
			side_vars[0]->next = NULL;
	}
}

int	unset(char **args, t_mshell *data)
{
	int	i;

	if (!data)
		return (-2);
	if (!args)
		return (custom_b_error("unset", "\x1C", "input not reachable", data));
	else if (!*args)
		return (update_var(data, "?", gc_itoa(0, data->gc, 1)));
	i = -1;
	while (args[++i])
		cut_var_from_env(args[i], data);
	return (0);
}
