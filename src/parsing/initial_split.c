/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:10:37 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/28 18:44:13 by sgoldenb         ###   ########.fr       */
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

	if (!c || !str)
		return (0);
	i = -1;
	while (str[++i])
		if (&str[i] == c)
			return (i);
	return (0);
}

static char	*insert_potential_vars(char *arg, t_mshell *data)
{
	int		i;
	int		j;
	char	*end_of_arg;
	char	*new_arg;
	char	*arg_value;
	char	*var_name;

	i = 0;
	if (!arg || !data)
		return (NULL);
	while (arg[i])
	{
		if (arg[i] == '$' && validate_var(&arg[i]) == TRUE)
		{
			j = i;
			j++;
			while (ft_isvarname(arg[j]) == TRUE)
				j ++;
			//DUP FIN D'ARG -> CPY VALUE -> JOIN FIN D'ARG
			end_of_arg = gc_strdup(&arg[j], data->gc, 1);
			var_name = gc_strnew(ft_lentillptr(&arg[j], &arg[i + 1]), data->gc, 1);
			var_name = ft_strncpy(var_name, &arg[i + 1], ft_lentillptr(&arg[j], &arg[i + 1]));
			printf("VARNAME = %s\n", var_name);
			arg_value = dup_var_value(data, var_name);
			printf("\n\n%s\n", arg_value);
			new_arg = gc_strnew(((i - (i - j)) + ft_strlen(dup_var_value(data, get_var_name(&arg[i], data))) + ft_strlen(end_of_arg)), data->gc, 1);
			new_arg = ft_strncpy(new_arg, arg, ft_lentillptr(&arg[i], arg));
			new_arg = gc_strjoin(new_arg, arg_value, data->gc, 1);
			new_arg = gc_strjoin(new_arg, end_of_arg, data->gc, 1);
			arg = gc_strdup(new_arg, data->gc, 1);
			i = 0;
		}
		i ++;
	}
	return (arg);
}
//MARCHE +++++


static char	*expand(char *arg, t_mshell *data)
{
	int		i;
	char	f_q;

	i = -1;
	while (arg[++i])
	{
		f_q = first_quoting(&arg[i], arg);
		printf("\nARGS[%d] F_Q:%c\t", i, f_q);
		if (f_q == '\'')
			continue ;
		else if (f_q == '\"' || f_q == 0)
			arg = insert_potential_vars(arg, data);
	}
	return (arg);
}

static char	**expand_all_args(char **args, t_mshell *data)
{
	int	i;

	if (!args || !*args || !data)
		return (NULL);
	i = -1;
	while (args[++i])
	{
		args[i] = expand(args[i], data);
		if (!args[i])
			return (NULL);
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
