/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:35:40 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/25 13:17:13 by sgoldenb         ###   ########.fr       */
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

	if (!data || !full_var)
		return (NULL);
	printf("%s\n", full_var);
	new = (t_envar *)gc_malloc(data->gc, sizeof(t_envar), 0);
	if (!new)
		return (NULL);
	new->next = NULL;
	new->hidden = FALSE;
	new->name = gc_strnew((ft_lentillc(full_var, '=') + 1), data->gc, 0);
	if (!new->name)
		return (NULL);
	new->name = ft_strncpy(new->name, full_var, ft_lentillc(full_var, '='));
	new->value = gc_strnew(ft_strlen(&full_var[ft_lentillc(full_var, '=') + 1]),
		data->gc, 0);
	if (!new->value)
		return (NULL);
	new->value = ft_strcpy(new->value, &full_var[ft_lentillc(full_var, '=')
		+ 1]);
	return (new);
}

static int	default_env(t_mshell *data)
{
	char	pwd[4096];

	if (!data)
		return (ENV_ERROR);
	if (!getcwd(pwd, sizeof(pwd)))
		return (ENV_ERROR);
	data->env = new_var("PWD", pwd, data, FALSE);
	if (!data->env)
		return (ENV_ERROR);
	data->env->next = new_var("SHLVL", "1", data, FALSE);
	if (!data->env->next)
		return (ENV_ERROR);
	return (ENV_FULL);
}

static int	create_hidden_vars(t_mshell *data)
{
	if (!data || !data->env)
		return (ENV_ERROR);
	get_last_var(data->env)->next = new_var("$0", "minishell", data, TRUE);
	get_last_var(data->env)->next = new_var("$?", "\0", data, TRUE);
	get_last_var(data->env)->next = new_var("\x1A", "", data, TRUE);
	if (!search_var(&data->env, "HOME"))
		get_last_var(data->env)->next = new_var("~", NULL, data, TRUE);
	else
		get_last_var(data->env)->next = new_var("~", search_var(&data->env,
				"HOME")->value, data, TRUE);
	return (ENV_FULL);
}

int	build_var_list(char **env, t_mshell *data)
{
	int	i;
	int	env_len;

	if (!data)
		return (ENV_ERROR);
	else if (!env || !*env)
		return (default_env(data));
	i = 0;
	env_len = ft_arrlen((void **)env);
	data->env = env_cpy(env[0], data);
	if (!data->env)
		return (ENV_ERROR);
	while (++i < env_len)
		(get_last_var(data->env)->next = env_cpy(env[i], data));
	update_var(data, "SHLVL", gc_itoa(ft_atoi(search_var(&data->env,
					"SHLVL")->value) + 1, data->gc, 0));
	create_hidden_vars(data);
	return (ENV_FULL);
}
