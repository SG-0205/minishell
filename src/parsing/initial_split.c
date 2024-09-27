/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:10:37 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/27 14:16:10 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	skip_limiter_name(char *hd_start)
{
	int		total_len;
	t_bool	name_start;

	if (!hd_start)
		return (0);
	total_len = -1;
	name_start = FALSE;
	while (hd_start[++total_len])
	{
		if ((ft_cisar(hd_start[total_len], " \t") == 1
				|| hd_start[total_len] == '<') && name_start == FALSE)
			continue ;
		if (ft_cisar(hd_start[total_len], " \t") == 1 && name_start == TRUE)
			break ;
		else
			name_start = TRUE;
	}
	printf("total_len = %d\n", total_len);
	return (total_len);
}

//APPROCHE INCOMPLETE! Il faut creer un fichier et rediriger stdin, se renseigner sur dup et dup2

char	*get_hd_limiter(char *hd_start, t_mshell *data)
{
	int		i;
	char	*limiter;
	int		limiter_len;

	if (!hd_start || !data)
		return (NULL);
	i = 1;
	limiter_len = 0;
	while (hd_start[++i])
	{
		if (ft_cisar(hd_start[i], " \t") == 1 && limiter_len == 0)
			continue ;
		else if (ft_cisar(hd_start[i], " \t") == 1 && limiter_len > 0)
			break ;
		else
			limiter_len ++;
	}
	limiter = gc_strnew(limiter_len, data->gc, 1);
	if (!limiter)
		return (NULL);
	limiter = ft_strncpy(limiter, &hd_start[i - limiter_len], limiter_len);
	return (limiter);
}

int	cpy_redir(char *red_start, char *new)
{
	int	i;
	
	if (!red_start || !new)
		return (0);
	i = -1;
	new = ft_strncat(new, RED_SEP, 1);
	while (red_start[++i] && ft_cisar(red_start[i], "<>") == 1)
		new = ft_strncat(new, &red_start[i], 1);
	new = ft_strncat(new, R_S_SEP, 1);
	while (red_start[i] && ft_cisar(red_start[i], " ") == 1)
		i ++;
	return (i);
}

int	read_redirection(char *red_start, t_mshell *data, char *new, char *str)
{
	int	i;
	
	if (!red_start || !data || !new)
		return (0);
	i = cpy_redir(red_start, new);
	while (red_start[i])
	{
		if (ft_cisar(red_start[i], "<>| \t") == 1
			&& (is_quoted_by('\'', &red_start[i], str) == FALSE
				&& is_quoted_by('\'', &red_start[i], str) == FALSE))
			break ;
		new = ft_strncat(new, &red_start[i], 1);
		i ++;
	}
	new = ft_strncat(new, RED_SEP, 1);
	printf("|%s * %s * %d|\n",red_start, new, i);
	return (i - 1);
}

char	*mark_redirections(char *input, t_mshell *data)
{
	int		i;
	char	*new;

	if (!input)
		return (NULL);
	i = -1;
	new = gc_strnew(ft_strlen(input) * ft_strlen(input), data->gc, 1);
	printf("%s -> input\n", input);
	while (input[++i])
	{
		if (ft_cisar(input[i], "<>") == 1 && is_quoted_by('\'', &input[i],
			input) == FALSE && is_quoted_by('\"', &input[i], input) == FALSE)
			i += read_redirection(&input[i], data, new, input);
		else
			new = ft_strncat(new, &input[i], 1);
	}
	return (new);
}

char	*args_separation(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
	{
		if ((ft_cisar(input[i], ARG_SEPARATORS) == 1 && is_quoted_by('\'',
					&input[i], input) == FALSE && is_quoted_by('\"', &input[i],
					input) == FALSE))
			input[i] = *ARG_SEP;
	}
	return (input);
}

int	ft_lentillptr(char *c, char *str)
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

char	**initial_split(char *input, t_mshell *data)
{
	char	**args;

	if (!input || !data)
		return (NULL);
	input = quote_closure_control(input);
	
	input = args_separation(input);
	
	args = gc_split(input, *ARG_SEP, data->gc, 0);
	if (!args || !*args)
		return (NULL);
	args = expand_all_args(args, data);
	if (!args)
		return (NULL);
	// args = detect_pipes_and_redirections(args, data);
	if (!args || !*args)
		return (NULL);
	return (args);
}
