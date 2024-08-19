/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:13:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/19 20:22:45 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_expand	*new_expansion(char *str, t_mshell *data)
{
	t_expand	*new;
	int			i;

	if (!str)
		return (NULL);
	i = -1;
	new = (t_expand *)gc_malloc(data->gc, sizeof(t_expand), 0);
	if (!new)
		return (NULL);
	new->to_expand = gc_strdup(str, data->gc, 0);
	if (!new->to_expand)
		return (NULL);
	count_quotes(new);
	while (MANAGED_QUOTES[++i] && i < 3)
		place_separator(new, MANAGED_QUOTES[i]);
}

int	count_pipes(char *raw_input)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (raw_input[++i])
		if (raw_input[i] == '|')
			count ++;
	return (count);
}

char	***make_cmds(char *raw_input, t_mshell *data)
{
	int	pipe_count;
	//TODO MALLOC SELON LE NOMBRE DE PIPES ET DE `ls`
}