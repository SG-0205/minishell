/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:39:41 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/24 09:22:55 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_envar	*get_var_source(t_envar *var, t_mshell *data)
{
	t_envar	*search;

	if (!var || !data)
		return (NULL);
	if (data->export)
		search = search_var(&data->export, var->name);
	if (search)
		return (data->export);
	search = search_var(&data->env, var->name);
	if (search)
		return (data->env);
	return (NULL);
}

t_envar	**get_side_vars(t_envar *var, t_mshell *data)
{
	t_envar	**side_vars;
	t_envar	*tmp;

	if (!var || !data || !data->env)
		return (NULL);
	tmp = get_var_source(var, data);
	side_vars = (t_envar **)gc_malloc(data->gc, sizeof(t_envar *) * 3, 1);
	if (!side_vars)
		return (NULL);
	side_vars[0] = NULL;
	side_vars[1] = NULL;
	side_vars[2] = NULL;
	while (tmp)
	{
		if (tmp->next == var)
			side_vars[0] = tmp;
		else if (tmp == var)
			side_vars[1] = tmp->next;
		if (side_vars[0] && side_vars[1])
			return (side_vars);
		tmp = tmp->next;
	}
	return (side_vars);
}

char	*dup_var_value(t_mshell *data, char *var_name)
{
	t_envar	*search_attempt;

	if (!data || !data->env || !var_name)
		return (NULL);
	search_attempt = search_var(&data->env, var_name);
	if (!search_attempt)
		return (NULL);
	else
		return (gc_strdup(search_attempt->value, data->gc, 0));
}

void	update_return_code(int exit_code, t_mshell *data)
{
	if (!data || !data->env)
		return ;
	update_var(data, "?", gc_itoa(exit_code, data->gc, 1));
}
