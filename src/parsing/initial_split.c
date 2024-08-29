/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:10:37 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/29 10:41:01 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static t_bool	is_escaped(char *c, char *str)
// {
// 	if (!c || !c - 1 || !str)
// 		return (FALSE);
// }

static char	*args_separation(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
	{
		if (ft_cisar(input[i], ARG_SEPARATORS) == 1
			&& is_quoted_by('\'', &input[i], input) == FALSE
			&& is_quoted_by('\"', &input[i], input) == FALSE)
			input[i] = *ARG_SEP;
	}
	return (input);
}

static int	ft_lentillptr(char *c, char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (&str[i] == c)
			return (i);
	return (ft_lentillc(str, *c));
}

static char	*extract_and_paste_value(char *var_start,
	char *var_end, t_mshell *data, char *arg)
{
	char	*new_arg;
	char	*arg_value;
	char	*arg_end;
	char	*var_name;

	if (!var_start || !data)
		return (NULL);
	arg_end = NULL;
	if (*var_end)
		arg_end = gc_strdup(var_end, data->gc, 1);
	var_name = gc_strnew(ft_lentillptr(var_end, var_start + 1), data->gc, 1);
	var_name = ft_strncpy(var_name, var_start + 1,
			ft_lentillptr(var_end, var_start + 1));
	if (!dup_var_value(data, var_name))
		arg_value = dup_var_value(data, "\x1A");
	else
		arg_value = dup_var_value(data, var_name);
	new_arg = gc_strnew((ft_strlen(arg) - (ft_lentillptr(var_end, var_start +1))
				+ ft_strlen(arg_value) + ft_strlen(var_end)), data->gc, 1);
	new_arg = ft_strncpy(new_arg, arg, ft_lentillptr(var_start, arg));
	new_arg = gc_strjoin(new_arg, arg_value, data->gc, 1);
	if (arg_end)
		new_arg = gc_strjoin(new_arg, arg_end, data->gc, 1);
	return (gc_strdup(new_arg, data->gc, 1));
}

static char	*insert_potential_vars(char *arg, t_mshell *data)
{
	int		i;
	int		j;

	i = 0;
	if (!arg || !data)
		return (NULL);
	while (arg[i])
	{
		if (arg[i] == '$' && validate_var(&arg[i]) == TRUE
			&& (first_quoting(&arg[i], arg) == '\"'
				|| first_quoting(&arg[i], arg) == 0))
		{
			printf("\nFQ[%c] = %c\n", arg[i], first_quoting(&arg[i], arg));
			j = i;
			j++;
			while (arg[j] && ft_isvarname(arg[j]) == TRUE)
				j ++;
			arg = extract_and_paste_value(&arg[i], &arg[j], data, arg);
			i = 0;
		}
		i ++;
	}
	return (arg);
}
//MARCHE +++++
//FONCTIONNE AVEC ET SANS QUOTE, S'OCCUPER DES "FAUSSES" VAR && SPLIT LA FCT <- EMPTY VARS OK
//PROBLEME SI ON FAIT DES TRUCS BIZARRES '"'"$TEST''''
//		Peut etre qu'en placant des separateurs?
//		Refaire first_quoting en partant du ptr et en faisant gauche/droite?

static char	*treat_consecutive_quoting(char *arg, t_mshell *data)
{
	int	i;

	if (!arg || !data)
		return (NULL);
	while (arg[++i])
	{
		//////////////////////TOFUCKINGDO
	}
}

static char	**expand_all_args(char **args, t_mshell *data)
{
	int		i;

	i = -1;
	while (args[++i])
	{
		args[i] = treat_consecutive_quoting(args[i], data)
		args[i] = insert_potential_vars(args[i], data);
	}
	return (args);
}

char	**initial_split(char *input, t_mshell *data)
{
	char	**args;

	if (!input || !data)
		return (NULL);
	args_separation(input);
	args = gc_split(input, *ARG_SEP, data->gc, 0);
	expand_all_args(args, data);
	if (!args || !*args)
		return (NULL);
	return (args);
}
