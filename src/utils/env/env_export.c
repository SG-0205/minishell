/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:42:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/24 09:06:04 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	env_size(t_envar **env_start, t_bool count_hidden)
{
	int		size;
	t_envar	*tmp;

	size = 0;
	if (!env_start || !*env_start)
		return (size);
	tmp = *env_start;
	while (tmp)
	{
		if (tmp->hidden == count_hidden)
			size ++;
		tmp = tmp->next;
	}
	return (size);
}

static t_envar	*get_first_var_on_visibility(t_envar **env_start,
	t_bool hidden, t_mshell *data)
{
	t_envar	*tmp;

	if (!data || !env_start || !*env_start)
		return (NULL);
	tmp = *env_start;
	while (tmp)
	{
		if (tmp->hidden == hidden)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_envar	**split_env_on_visibility(t_envar **env_start, t_mshell *data)
{
	t_envar	**splitted_list;
	t_envar	*tmp;

	if (!env_start || !*env_start)
		return (NULL);
	splitted_list = (t_envar **)gc_malloc(data->gc, (sizeof(t_envar *) * 3), 0);
	if (!splitted_list)
		return (NULL);
	splitted_list[SPLIT_END] = NULL;
	splitted_list[HIDDEN_VARS] = dup_var(get_first_var_on_visibility(env_start,
				TRUE, data), data);
	splitted_list[PUBLIC_VARS] = dup_var(get_first_var_on_visibility(env_start,
				FALSE, data), data);
	tmp = *env_start;
	while (tmp)
	{
		if (tmp->hidden == TRUE)
			(get_last_var(splitted_list[HIDDEN_VARS])->next
					= dup_var(tmp, data));
		else
			(get_last_var(splitted_list[PUBLIC_VARS])->next
					= dup_var(tmp, data));
		tmp = tmp->next;
	}
	return (splitted_list);
}

char	*join_var_name_and_value(t_envar *var, t_mshell *data)
{
	char	*new;
	int		new_size;

	if (!var || !data)
		return (NULL);
	new_size = ft_strlen(var->name) + ft_strlen(var->value) + 1;
	new = (char *)gc_malloc(data->gc, (sizeof(char *) * new_size), 0);
	ft_bzero(new, new_size);
	new[new_size] = 0;
	ft_strcat(new, var->name);
	ft_strcat(new, "=\0");
	ft_strcat(new, var->value);
	return (new);
}

char	**env_list_to_array(t_mshell *data, t_bool hidden)
{
	char	**converted_env;
	t_envar	**splitted_env;
	t_envar	*tmp;
	int		i;

	i = 0;
	if (!data)
		return (NULL);
	converted_env = (char **)gc_malloc(data->gc, (sizeof(char *)
				* (env_size(&data->env, hidden) + 1)), 0);
	if (!converted_env)
		return (NULL);
	converted_env[env_size(&data->env, hidden)] = NULL;
	splitted_env = split_env_on_visibility(&data->env, data);
	if (!splitted_env)
		return (NULL);
	tmp = splitted_env[hidden];
	while (tmp)
	{
		tmp = tmp->next;
		converted_env[i] = join_var_name_and_value(tmp, data);
		i ++;
	}
	return (converted_env);
}
