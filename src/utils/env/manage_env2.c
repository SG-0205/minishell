/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:39:41 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 15:43:53 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char	*dup_var_value(t_mshell *data, char *var_name)
{
	t_envar	*search_attempt;

	if (!data || !data->env || !var_name)
		return (NULL);
	search_attempt = search_var(&data->env, var_name);
	if (!search_attempt)
		return (NULL);
	else
		return (gc_strdup(search_attempt->value, data->gc, 0));
}
