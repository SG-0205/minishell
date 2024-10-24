/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:50:05 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/24 09:15:13 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*export_pretty_print(char *full_var, t_mshell *data)
{
	char	**split;
	char	*name;
	char	*value;
	char	*final;

	if (!data || !full_var)
		return (NULL);
	split = gc_split(full_var, '=', data->gc, 1);
	if (!split || !*split)
		return (NULL);
	if (!split[1])
		return (split[0]);
	name = gc_strdup(split[0], data->gc, 1);
	if (!name)
		return (NULL);
	value = gc_strnew(ft_strlen(split[1]) + 2, data->gc, 1);
	if (!value)
		return (NULL);
	value = ft_strncat(value, "\"", 1);
	value = ft_strncat(value, split[1], ft_strlen(split[1]));
	value = ft_strncat(value, "\"\0", 2);
	final = gc_strjoin(name, gc_strjoin("=\0", value, data->gc, 1), data->gc, 1);
	if (!final)
		return (NULL);
	return (final);
}

int	exp_size(t_envar **exp_start)
{
	int		size;
	t_envar	*tmp;

	if (!exp_start || !*exp_start)
		return (0);
	size = 0;
	tmp = *exp_start;
	while (tmp)
	{
		if (tmp->hidden != ERROR)
			size ++;
		tmp = tmp->next;
	}
	return (size);
}

char	**exp_list_to_array(t_envar **exp_list, t_mshell *data)
{
	t_envar *tmp;
	char	**converted;
	int		i;

	if (!exp_list || !*exp_list || !data)
		return (NULL);
	tmp = *exp_list;
	converted = (char **)gc_malloc(data->gc,
		(sizeof(char *) * (exp_size(exp_list) + 1)), 1);
	if (!converted)
		return (NULL);
	i = 0;
	while (tmp)
	{
		if (tmp->hidden != ERROR)
			converted[i++] = gc_strdup(tmp->name, data->gc, 1);
		tmp = tmp->next;
		// printf("CONVEXP %s\n", converted[i]);
	}
	converted[i] = NULL;
	return (converted);
}

char	**cat_lists(char **env, char **exp, t_mshell *data)
{
	char	**final;
	int		final_size;
	int		i;
	int		j;

	if (!env || !*env)
		return (NULL);
	final_size = ft_arrlen((void **)env) + ft_arrlen((void **)exp);
	final = (char **)gc_malloc(data->gc,
		(sizeof(char *) * final_size), 1);
	i = -1;
	j = 0;
	while (++i < final_size)
	{
		if (i < final_size - ft_arrlen((void **)exp) && env[i])
			final[i] = gc_strdup(env[i], data->gc, 1);
		else if (i + j < final_size && exp && exp[j])
			final[i] = gc_strdup(exp[j++], data->gc, 1);
	}
	final[i] = NULL;
	return (final);
}

static void	special_print(t_mshell *data)
{
	char	**splitted_env;
	char	**splitted_exp;
	int		i;

	if (!data)
		return ;
	splitted_env = env_list_to_array(data, FALSE);
	if (!splitted_env || !*splitted_env)
		return ;
	splitted_exp = exp_list_to_array(&data->export, data);
	i = -1;
	while (splitted_env[++i])
		printf("export %s\n", export_pretty_print(splitted_env[i], data));
	i = -1;
	while (splitted_exp && splitted_exp[++i])
		printf("export %s\n", export_pretty_print(splitted_exp[i], data));
}

static t_bool	check_args(char *args)
{
	int	i;

	i = -1;
	if (ft_isdigit(args[0]) != 0 || ft_strcmp(args, "=\0") == 0
		|| args[0] == '=')
		return (FALSE);
	while (args[++i] && args[i] != '=')
	{
		if (ft_cisar(args[i], EXPORT_FORBIDDEN_CHARS) == 1
			|| ft_cisar(args[i], "?") == 1)
			return (FALSE);
	}
	return (TRUE);
}

t_envar	*update_export(char *name, t_mshell *data)
{
	t_envar	*new;

	if (!data || !name)
		return (NULL);
	if (search_var(&data->export, name))
		return (NULL);
	new = new_var(name, NULL, data, TRUE);
	if (!data->export)
		data->export = new;
	else
		get_last_var(data->export)->next = new;
	return (data->export);
}

t_bool	single_value(char *arg)
{
	int	i;

	if (!arg)
		return (FALSE);
	i = -1;
	while (arg[++i])
		if (arg[i] == '=')
			return (FALSE);
	return (TRUE);
}

static void	update_env(char *arg, t_mshell *data)
{
	char	*name;
	char	*value;

	if (!arg || !data)
		return ;
	name = gc_strnew(ft_lentillc(arg, '='), data->gc, 1);
	if (!name)
		return ;
	name = ft_strncpy(name, arg, ft_lentillc(arg, '='));
	value = gc_strnew(ft_strlen(&arg[ft_lentillc(arg, '=') + 1]), data->gc, 1);
	if (!value)
		return ;
	value = ft_strncpy(value, &arg[ft_lentillc(arg, '=') + 1],
			ft_strlen(&arg[ft_lentillc(arg, '=') + 1]));
	if (ft_strcmp(value, "\0") == 0)
	{
		update_export(name, data);
		return ;
	}
	update_var(data, name, value);
}

int	export(t_mshell *data, char **args)
{
	int	i;
	int retcode;

	if (!data)
		return (NODATA);
	if (!args || !*args)
	{
		special_print(data);
		return (0);
	}
	retcode = 0;
	i = -1;
	while (args[++i])
		if (check_args(args[i]) == FALSE)
			retcode = custom_b_error("export", args[i],
				"not a valid identifier", data);
		else if (single_value(args[i]) == FALSE)
			update_env(args[i], data);
		else
			update_export(args[i], data);
	return (retcode);
}
