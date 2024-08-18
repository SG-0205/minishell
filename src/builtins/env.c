/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:16:09 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/18 23:16:52 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_env(t_mshell *data)
{
	t_envar	*tmp;

	if (!data || !data->env)
		return ;
	tmp = data->env;
	while (tmp)
		(printf("%s=%s\n", tmp->name, tmp->value), tmp = tmp->next);
}
