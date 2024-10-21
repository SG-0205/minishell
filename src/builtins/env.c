/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:16:09 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/06 17:00:55 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	print_env(t_mshell *data, t_envsplit to_print)
{
	t_envar	*tmp;

	if (!data || !data->env)
		return (1);
	tmp = data->env;
	while (tmp)
	{
		if (to_print == SPLIT_END)
			printf("%s=%s\n", tmp->name, tmp->value);
		else if (to_print == PUBLIC_VARS && tmp->hidden == FALSE)
			printf("%s=%s\n", tmp->name, tmp->value);
		else if (to_print == HIDDEN_VARS && tmp->hidden == TRUE)
			printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
