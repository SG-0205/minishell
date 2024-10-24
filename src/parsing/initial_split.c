/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:10:37 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/22 22:32:16 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	try_insert_value(char *var_start, char *container, t_mshell *data)
{
	t_envar	*search_try;
	int		value_len;

	if (!var_start || !container || !data)
		return (0);
	value_len = 0;
	search_try = search_var(&data->env, get_var_name(var_start, data));
	if (search_try)
	{
		value_len = ft_strlen(search_try->value);
		ft_strncat(container, search_try->value, value_len);
	}
	return (value_len - ft_strlen(get_var_name(var_start, data)));
}

int	compute_exp_size(char *arg, t_mshell *data)
{
	int		size;
	t_envar	*search_try;
	int		i;

	if (!arg || !data)
		return (0);
	i = -1;
	size = 0;
	while (arg[++i])
	{
		if (arg[i] == '$' && validate_var(&arg[i]) == TRUE
			&& is_quoted_by('\"', &arg[i], arg) == TRUE
			&& is_quoted_by('\'', &arg[i], arg) == FALSE)
		{
			search_try = search_var(&data->env, get_var_name(&arg[i], data));
			if (!search_try)
				size ++;
			else
				size += ft_strlen(search_try->value);
		}
		else
			size ++;
	}
	return (size + i);
}

int	insert_var(char *var_start, char *arg, t_mshell *data, char *n_arg)
{
	if (!var_start || !arg || !data || !n_arg)
		return (0);
	try_insert_value(var_start, n_arg, data);
	return ((int)ft_strlen(get_var_name(var_start, data)));
}

char	*read_quoting(char *arg, t_mshell *data)
{
	int		i;
	int		expanded_size;
	char	*new_arg;

	i = -1;
	expanded_size = compute_exp_size(arg, data);
	if (!arg || !data)
		return (NULL);
	new_arg = gc_strnew(expanded_size * expanded_size + 1, data->gc, 1);
	if (!new_arg)
		return (NULL);
	while (arg[++i])
	{
		if (arg[i] == '\"' && is_closed(&arg[i], arg) == TRUE)
			i += read_dq(&arg[i], arg, new_arg, data);
		else if (arg[i] == '\'' && is_closed(&arg[i], arg) == TRUE)
			i += read_sq(&arg[i], arg, new_arg);
		else if (arg[i] == '$' && validate_var(&arg[i]) == TRUE
			&& is_quoted(&arg[i], arg, TRUE) == FALSE)
			i += insert_var(&arg[i], arg, data, new_arg);
		else
			ft_strncat(new_arg, &arg[i], 1);
	}
	return (new_arg);
}

t_bool	transparent_arg(char *arg)
{
	int	i;

	if (!arg)
		return (FALSE);
	i = -1;
	while (arg[++i])
		if (ft_cisar(arg[i], SEPS) != 0)
			return (TRUE);
	return (FALSE);
}

char	**args_rotation(char **args, int *cmd_id, t_mshell *data)
{
	int		i;

	if (!args || !data)
		return (NULL);
	i = -1;
	if (**args || empty_line(args[0]) == FALSE
		|| has_redirs_by_id(cmd_id, data->redir_link) == TRUE)
		return (args);
	while (args[++i])
	{
		// printf("[%d]%s\n", i, args[i]);
		if (i > 0 && args[i])
			args[i - 1] = gc_strdup(args[i], data->gc, 1);
	}
	args[i - 1] = NULL;
	return (args);
}

char	**initial_split(char *input, int *cmd_id, t_mshell *data)
{
	char	**args;

	if (!input || !data)
		return (NULL);
	input = quote_closure_control(input);
	input = args_separation(input);
	args = gc_split(input, *ARG_SEP, data->gc, 0);
	if (!args || (!*args && has_redirs_by_id(cmd_id, data->redir_link) == FALSE))
		return (NULL);
	args = expand_all_args(args, data);
	if (!args)
		return (NULL);
	// printf("%p\n", args);
	if (has_redirs_by_id(cmd_id, data->redir_link) == FALSE && empty_line(*args) == TRUE)
		args = args_rotation(args, cmd_id, data);
	return (args);
}
