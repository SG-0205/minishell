/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_manipulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:35:23 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/30 11:39:59 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	str_shrink(char *str)
{
	int	i;

	i = -1;
	while (str[++i + 1])
		str[i] = str[i + 1];
	str[i] = 0;
}

char	*str_insert(char *str, char *where_to_insert, char *to_insert,
	t_mshell *data)
{
	int		i;
	char	*new;

	if (!str || !where_to_insert || !to_insert)
		return (NULL);
	i = -1;
	while (str[++i])
		if (&str[i] == where_to_insert)
			break ;
	if (str[i] == 0 || &str[i] != where_to_insert)
		return (gc_strjoin(str, to_insert, data->gc, 1));
	new = gc_strnew(ft_lentillptr(&str[i], str)
			+ ft_strlen(&str[i]) + ft_strlen(to_insert), data->gc, 1);
	if (!new)
		return (NULL);
	new = ft_strncpy(new, str, ft_lentillptr(&str[i], str));
	new = ft_strncpy(new, to_insert, ft_strlen(to_insert));
	new = ft_strncpy(new, &str[i], ft_strlen(&str[i]));
	return (new);
}
