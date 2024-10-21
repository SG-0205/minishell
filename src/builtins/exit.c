/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:56:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/08 14:29:45 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

long long int	ft_atol(const char *str)
{
	long long int	result;
	int				digit;
	int				sign;

	sign = 1;
	result = 0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		result = result * 10 + digit;
		str++;
	}
	return (sign * result);
}

static t_bool	check_arg(char *arg)
{
	int	i;

	if (!arg)
		return (FALSE);
	i = -1;
	while (arg[++i])
		if (ft_cisar(arg[i], "+-0123456789") == 0)
			return (FALSE);
	if (ft_strlen(arg) > 20 && (ft_atol(arg) > INT_MAX
			|| ft_atol(arg) < INT_MIN))
		return (FALSE);
	if (ft_atol(arg) > LONG_MAX)
		return (FALSE);
	return (TRUE);
}

int	exit_b(char **args, t_mshell *data)
{
	long long int	exit_code;

	if (!data || !args)
		return (-1);
	if (ft_arrlen((void **)args) > 1)
	{
		printf("exit\n");
		custom_shell_error("exit\0", strerror(E2BIG), 1, data);
		update_var(data, "?", gc_itoa(1, data->gc, 1));
		return (1);
	}
	exit_code = 0;
	if (!*args)
		exit_code = ft_atoi(search_var(&data->env, "?")->value);
	else
		exit_code = ft_atol(*args);
	if (check_arg(*args) == FALSE
		|| exit_code > LONG_MAX || exit_code < LONG_MIN)
	{
		exit_code = custom_shell_error(gc_strjoin("exit: ", *args, data->gc, 1),
				"numeric argument required", 2, data);
	}
	printf("exit\n");
	clear_data(data);
	exit(normalize_return(exit_code));
}

//Verifier L_MAX