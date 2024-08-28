/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 22:41:42 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/28 18:32:58 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_var_name(char *var_start, t_mshell *data)
{
	int		i;
	char	*name;

	i = 0;
	if (var_start[i] != '$')
		return (NULL);
	i++;
	if (ft_cisar(var_start[i], EXPORT_FORBIDDEN_CHARS) != 0
		|| ft_cisar(var_start[i], "123456789 ") != 0)
		return (NULL);
	while (var_start[++i])
	{
		if (ft_isvarname(var_start[i]) == FALSE)
			break ;
	}
	name = gc_strnew(ft_lentillc(var_start + 1, var_start[i]), data->gc, 1);
	if (!name)
		return (NULL);
	name = ft_strncpy(name, var_start + 1, ft_lentillc(var_start + 1,
				var_start[i]));
	return (name);
}

static t_envar	**store_vars(t_mshell *data, char **names, t_expand *str)
{
	t_envar	**new;
	int		i;
	t_envar	*search_attempt;

	i = -1;
	if (!data || !names || !*names)
		return (NULL);
	new = (t_envar **)gc_malloc(data->gc, (sizeof(t_envar *)
				* (str->var_count + 1)), 1);
	if (!new)
		return (NULL);
	while (++i <= str->var_count)
		new[i] = NULL;
	i = -1;
	while (++i < str->var_count && names[i])
	{
		search_attempt = search_var(&data->env, names[i]);
		if (!search_attempt)
			new[i] = search_var(&data->env, "\x1A");
		else
			new[i] = search_attempt;
	}
	return (new);
}

static int	count_vars(char *str, t_mshell *data)
{
	int	i;
	int	var_count;

	i = -1;
	var_count = 0;
	if (!str || !data)
		return (var_count);
	while (str[++i])
	{
		if (str[i] == '$' && is_quoted_by(*SQ_SEP, &str[i], str) == FALSE
			&& validate_var(&str[i]) == TRUE)
			var_count ++;
	}
	return (var_count);
}

int	mark_vars(t_expand *str, t_mshell *data)
{
	int		var_count;
	char	**var_names;

	if (!str || !str->expanded || !data)
		return (VARS_ERROR);
	str->var_count = count_vars(str->expanded, data);
	if (str->var_count < 1)
		return (VARS_NONE);
	var_count = str->var_count;
	print_exp(str, "MARK_VARS");
	var_names = (char **)gc_malloc(data->gc,
			(sizeof(char *) * (str->var_count + 1)), 1);
	if (!var_names)
		return (VARS_ERROR);
	var_names = fill_var_names(str->expanded, var_count, data, var_names);
	str->vars_to_insert = store_vars(data, var_names, str);
	if (!str->vars_to_insert)
		return (VARS_ERROR);
	place_var_sep(str, data);
	return (VARS_FOUND);
}
