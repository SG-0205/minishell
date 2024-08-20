/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:04:21 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/20 15:50:09 by sgoldenb         ###   ########.fr       */
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

static char	*quote_args(char *args, t_mshell *data)
{
	if (!args)
		return (NULL);
	if (enable_arg_quoting(args) == FALSE)
		return (args);
	args = gc_strjoin("`", args, data->gc, 0);
	if (!args)
		return (NULL);
	args = gc_strjoin(args, "': ", data->gc, 0);
	if (!args)
		return (NULL);
	return (args);
}

int	builtin_error(char *builtin_name, char *args, int errnum, t_mshell *data)
{
	char	*error_msg;

	error_msg = quote_args(args, data);
	error_msg = gc_strjoin(gc_strjoin(builtin_name, ": ", data->gc, 0),
			error_msg, data->gc, 0);
	error_msg = gc_strjoin("minishell: ", error_msg, data->gc, 0);
	error_msg = gc_strjoin(error_msg, strerror(errnum), data->gc, 0);
	error_msg = gc_strjoin(error_msg, "\n", data->gc, 0);
	write(2, error_msg, ft_strlen(error_msg));
	return (errnum);
}
