/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:49:55 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/20 11:01:50 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_envar	*new_var(char *name, char *value, t_mshell *data)
{
	t_envar	*new;

	if (!name)
		return (NULL);
	new = (t_envar *)gc_malloc(data->gc, sizeof(t_envar), 0);
	if (!new)
		return (NULL);
	new->name = gc_strdup(name, data->gc, 0);
	if (!new->name)
		return (NULL);
	new->value = gc_strdup(value, data->gc, 0);
	if (!new->name)
		return (NULL);
	new->next = NULL;
	return (new);
}

t_envar	*search_var(t_envar **first_var, char *var_name)
{
	t_envar	*tmp;

	if (!var_name || !first_var || !*first_var)
		return (NULL);
	tmp = *first_var;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, var_name) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	update_var(t_mshell *data, char *name, char *new_value)
{
	t_envar	*tmp;

	if (!data || !data->env || !data->env || !name)
		return (MOD_KO);
	tmp = search_var(&data->env, name);
	if (!tmp)
	{
		get_last_var(data->env)->next = new_var(name, new_value, data);
		return (MOD_OK);
	}
	if (!new_value)
		tmp->value = "";
	else
		tmp->value = gc_strdup(new_value, data->gc, 0);
	if (!tmp->value)
		return (MOD_KO);
	return (MOD_OK);
}
