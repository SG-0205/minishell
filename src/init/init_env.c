/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:35:40 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/18 22:17:36 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_envar	*get_last_var(t_envar *start)
{
	t_envar	*tmp;

	if (!start)
		return (NULL);
	tmp = start;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_envar	*env_cpy(char *full_var, t_mshell *data)
{
	t_envar	*new;

	if (!data)
		return (NULL);
	new = (t_envar *)gc_malloc(data->gc, sizeof(t_envar), 0);
	if (!new)
		return (NULL);
	new->next = NULL;
	new->name = gc_strnew((ft_lentillc(full_var, '=') + 1), data->gc, 0);
	if (!new->name)
		return (NULL);
	new->name = ft_strncpy(new->name, full_var, ft_lentillc(full_var, '='));
	new->value = gc_strnew(ft_strlen(&full_var[ft_lentillc(full_var, '=') + 1]),
			data->gc, 0);
	if (!new->value)
		return (NULL);
	new->value = ft_strcpy(new->value, &full_var[ft_lentillc(full_var, '=') + 1]);
	return (new);
}

int	build_var_list(char **env, t_mshell *data)
{
	int	i;
	int	env_len;
	t_envar	tmp;

	if (!data)
		return (ENV_ERROR);
	else if (!env || !*env)
		return (ENV_EMPTY);
	i = 0;
	env_len = ft_arrlen((void **)env);
	data->env = env_cpy(env[0], data);
	while (++i < env_len)
		get_last_var(data->env)->next = env_cpy(env[i], data);
	return (ENV_FULL);
}
