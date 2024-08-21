/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_vars_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:42:33 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/21 13:00:46 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**fill_var_names(char *to_expand, int var_count,
	t_mshell *data, char **names)
{
	int	i;
	int	j;

	if (!to_expand || !data || !names)
		return (NULL);
	i = -1;
	j = 0;
	while (to_expand[++i] && var_count > 0)
	{
		if (to_expand[i] == '$'
			&& validate_var(&to_expand[i]) == TRUE)
		{
			names[j] = get_var_name(&to_expand[i], data);
			if (!names[j])
				return (NULL);
			j ++;
			var_count --;
		}
	}
	return (names);
}
