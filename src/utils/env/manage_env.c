/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:49:55 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/21 14:00:45 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	var_list_position(t_envar **first_var, t_envar *var)
{
	t_envar	*tmp;
	int		position;

	if (!var || !first_var || !*first_var)
		return (-1);
	tmp = *first_var;
	position = 0;
	while (tmp)
	{
		if (tmp == var)
			return (position);
		position ++;
		tmp = tmp->next;
	}
	return (-1);
}

t_envar	*new_var(char *name, char *value, t_mshell *data, t_bool hide)
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
	if (!value)
		new->value = NULL;
	else
		new->value = gc_strdup(value, data->gc, 0);
	if (!new->name)
		return (NULL);
	new->next = NULL;
	new->hidden = hide;
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
		(get_last_var(data->env)->next = new_var(name, new_value, data, FALSE));
		return (0);
	}
	if (!new_value)
		tmp->value = "\0";
	else
		tmp->value = gc_strdup(new_value, data->gc, 0);
	if (!tmp->value)
		return (MOD_KO);
//
	return (0);
}

t_envar	*dup_var(t_envar *var, t_mshell *data)
{
	t_envar	*dup;

	if (!data || !data->gc || !var)
		return (NULL);
	dup = new_var(var->name, var->value, data, var->hidden);
	if (!dup)
		return (NULL);
	return (dup);
}
