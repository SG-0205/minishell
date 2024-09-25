/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:13:29 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/24 15:03:38 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	print_s_error(char *p_err, t_mshell *data, int errnum)
{
	if (!p_err || !data)
		return (-1);
	write(2, p_err, ft_strlen(p_err));
	return (errnum);
}

static char	*add_strerror(char *p_err, int errnum, t_mshell *data,
	char *error_message)
{
	if (error_message && !errnum)
	{
		p_err = gc_strnew((ft_strlen(error_message)
				+ ft_strlen("minishell: ")) , data->gc, 1);
		if (!p_err)
			return (p_err);
		p_err = ft_strncat(p_err, "minishell: ", ft_strlen("minishell: "));
		p_err = ft_strncat(p_err, error_message, ft_strlen(error_message));
	}
	else if (!error_message && errnum)
	{
		p_err = gc_strnew(ft_strlen(strerror(errnum)
				+ ft_strlen("minishell: ")), data->gc, 1);
		if (!p_err)
			return (p_err);
		p_err = ft_strncat(p_err, "minishell: ", ft_strlen("minishell: "));
		p_err = ft_strncat(p_err, strerror(errnum),
			ft_strlen(strerror(errnum)));
	}
	return (p_err);
}

int	syntax_error(char *error_message, int errnum, t_mshell *data)
{
	char	*p_err;

	if ((!error_message && !errnum) || !data)
		return (0);
	p_err = gc_strnew(1, data->gc, 1);
	p_err = add_strerror(p_err, errnum, data, error_message);
	if (*p_err == 0 || ft_strlen(p_err) < ft_strlen("minishell: "))
		p_err = gc_strjoin("minishell: ", error_message, data->gc, 1);
	return (print_s_error(p_err, data, errnum));
}
