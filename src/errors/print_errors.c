/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:04:21 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/27 15:28:23 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	enable_arg_quoting(char *args)
{
	int	i;

	if (!args)
		return (FALSE);
	i = -1;
	while (args[++i])
		if (ft_isalnum(args[i]) == 0)
			return (TRUE);
	return (FALSE);
}

char	*quote_e_args(char *args, t_bool enforce, t_mshell *data)
{
	char	*new_arg;

	if (!args)
		return (NULL);
	if (enable_arg_quoting(args) == FALSE && enforce == FALSE)
		return (gc_strjoin(args, ": \0", data->gc, 1));
	new_arg = gc_strnew(ft_strlen(args) + 4, data->gc, 1);
	new_arg = ft_strcat(new_arg, "`\0");
	new_arg = ft_strcat(new_arg, args);
	new_arg = ft_strcat(new_arg, "\'\0");
	new_arg = ft_strcat(new_arg, ": \0");
	return (new_arg);
}

int	builtin_error(char *builtin_name, char *args, int errnum, t_mshell *data)
{
	char	*error_msg;

	if (!builtin_name || !args || !data)
		return (errnum);
	update_var(data, "$?", gc_itoa(errnum, data->gc, 1));
	args = quote_e_args(read_quoting(args, data), FALSE, data);
	error_msg = gc_strnew(error_full_len((char *[]){"minishell: \0", args,
				strerror(errnum), builtin_name, NULL}) + 1, data->gc, 1);
	if (!error_msg)
		return (ENOMEM);
	error_msg = ft_strcat(error_msg, "minishell: \0");
	error_msg = ft_strcat(error_msg, builtin_name);
	error_msg = ft_strcat(error_msg, args);
	error_msg = ft_strcat(error_msg, strerror(errnum));
	error_msg = ft_strcat(error_msg, "\n\0");
	write(2, error_msg, ft_strlen(error_msg));
	return (errnum);
}

int	mshell_error(char *faulty_arg, int errnum, t_mshell *data)
{
	char	*error_msg;

	if (!faulty_arg || !data)
		return (errnum);
	update_var(data, "$?", gc_itoa(errnum, data->gc, 1));
	faulty_arg = quote_e_args(read_quoting(faulty_arg, data), FALSE, data);
	error_msg = gc_strnew(error_full_len((char *[]){"minishell: \0",
				faulty_arg, strerror(errnum), NULL}) + 1, data->gc, 1);
	if (!error_msg)
		return (ENOMEM);
	error_msg = ft_strcat(error_msg, "minishell: \0");
	error_msg = ft_strcat(error_msg, faulty_arg);
	error_msg = ft_strcat(error_msg, strerror(errnum));
	error_msg = ft_strcat(error_msg, "\n\0");
	write(2, error_msg, ft_strlen(error_msg));
	return (errnum);
}

int	custom_b_error(char *builtin_name, char *args,
	char *custom_msg, t_mshell *data)
{
	char	*error_msg;

	if (args)
		error_msg = quote_e_args(args, FALSE, data);
	else
		error_msg = gc_strnew(1, data->gc, 1);
	error_msg = gc_strjoin(gc_strjoin(builtin_name, ": ", data->gc, 1),
			error_msg, data->gc, 1);
	error_msg = gc_strjoin("minishell: ", error_msg, data->gc, 1);
	if (args && enable_arg_quoting(args) == FALSE)
		error_msg = gc_strjoin(error_msg, ": ", data->gc, 1);
	error_msg = gc_strjoin(error_msg, custom_msg, data->gc, 1);
	error_msg = gc_strjoin(error_msg, "\n", data->gc, 1);
	write(2, error_msg, ft_strlen(error_msg));
	return (-1);
}
