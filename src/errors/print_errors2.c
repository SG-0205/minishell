/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:13:29 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/26 16:15:41 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	error_full_len(char **args)
{
	int	i;
	int	len;

	if (!args || !*args)
		return (0);
	i = -1;
	len = 0;
	while (args[++i])
		len += ft_strlen(args[i]);
	return (len);
}

int	bad_eof(char *limiter, int l_count, t_mshell *data)
{
	char	*error_msg;
	char	*l_itoa;

	if (!limiter || !data)
		return (258);
	l_itoa = gc_itoa(l_count, data->gc, 1);
	if (!l_itoa)
		return (258);
	update_var(data, "$?", gc_itoa(258, data->gc, 1));
	limiter = quote_e_args(limiter, TRUE, data);
	error_msg = gc_strnew(error_full_len((char *[]){"minishell: \0", limiter,
				l_itoa, "warning: here-document at line  \0",
				"delimited by end-of-file (wanted )\n\0", NULL}), data->gc, 1);
	error_msg = ft_strcat(error_msg,
		"minishell: warning: here-document at line \0");
	error_msg = ft_strcat(error_msg, l_itoa);
	error_msg = ft_strcat(error_msg, " delimited by end-of-file (wanted \0");
	error_msg = ft_strcat(error_msg, limiter);
	error_msg = ft_strcat(error_msg, ")\n\0");
	write(2, error_msg, ft_strlen(error_msg));
	return (258);
}

int	syntax_error(char *faulty_token, t_mshell *data)
{
	char	*error_msg;

	if (!data)
		return (258);
	if (!faulty_token)
		faulty_token = gc_strdup("newline", data->gc, 1);
	update_var(data, "$?", gc_itoa(258, data->gc, 1));
	faulty_token = quote_e_args(faulty_token, TRUE, data);
	error_msg = gc_strnew(error_full_len((char *[]){"minishell: ",
				"syntax error near unexpected token \n\0", faulty_token}),
				data->gc, 1);
	error_msg = ft_strcat(error_msg,
		"minishell: syntax error near unexpected token \0");
	error_msg = ft_strcat(error_msg, faulty_token);
	error_msg = ft_strcat(error_msg, "\n\0");
	write(2, error_msg, ft_strlen(error_msg));
	return (258);
}