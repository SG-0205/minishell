/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 08:54:53 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/21 16:14:25 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	is_a_flag(char *arg)
{
	int		i;

	if (!arg || !*arg)
		return (FALSE);
	i = 1;
	if (ft_strncmp(arg, "-n", 2) != 0)
		return (FALSE);
	while (arg[++i])
		if (arg[i] != 'n')
			return (FALSE);
	return (TRUE);
}

static char	*cat_and_sep(char **args, char *echo_str)
{
	int		i;

	if (!args || !echo_str)
		return (NULL);
	i = -1;
	while (args[++i])
	{
		if ((i == 0 && is_a_flag(args[i]) == TRUE) || (args[i - 1]
				&& is_a_flag(args[i - 1]) == TRUE
				&& is_a_flag(args[i]) == TRUE) || !*args)
			continue ;
		else if (*args[i] && ft_strlen(args[i]) > 0)
		{
			echo_str = ft_strncat(echo_str, args[i], ft_strlen(args[i]));
			if (args[i + 1] && args[i + 1][0])
				echo_str = ft_strncat(echo_str, " \0", 1);
		}
	}
	return (echo_str);
}

static char	*join_args(char **args, t_mshell *data)
{
	char	*to_print;
	int		total_size;
	int		i;

	if (!args || !data)
		return (NULL);
	else if (!*args)
		return (gc_strnew(1, data->gc, 1));
	i = -1;
	total_size = 0;
	while (args[++i])
	{
		if ((i == 0 && is_a_flag(args[i]) == TRUE) || (args[i - 1]
				&& is_a_flag(args[i - 1]) == TRUE
				&& is_a_flag(args[i]) == TRUE))
			continue ;
		else
			total_size += ft_strlen(args[i]);
	}
	to_print = gc_strnew(total_size + i + 1, data->gc, 1);
	if (!to_print)
		return (NULL);
	to_print = cat_and_sep(args, to_print);
	return (to_print);
}

int	echo_b(char **args, t_mshell *data)
{
	t_bool	no_nl_flag;
	char	*cat_args;

	if (!args || !data)
		return (-1);
	no_nl_flag = FALSE;
	if (is_a_flag(*args) == TRUE)
		no_nl_flag = TRUE;
	cat_args = join_args(args, data);
	if (!cat_args)
		return (-1);
	printf("%s", cat_args);
	if (no_nl_flag == FALSE)
		printf("\n");
	return (0);
}
