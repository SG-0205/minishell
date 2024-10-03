/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:56:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/03 14:50:22 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exit_b(char **args, t_mshell *data)
{
	int	exit_code;

	if (!data || !args)
		return (-1);
	if (ft_arrlen((void **)args) > 1)
	{
		printf("exit\n");
		return (custom_shell_error("exit\0", strerror(E2BIG), 1, data));
	}
	if (!*args)
		exit_code = ft_atoi(search_var(&data->env, "?")->value);
	else
		exit_code = ft_atoi(*args);
	printf("exit");
	clear_data(data);
	exit(exit_code);
}
