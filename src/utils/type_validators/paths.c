/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:17:26 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/26 15:33:20 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_bool	is_path_char(char c)
{
	if (ft_isalnum(c) != 0 || ft_cisar(c, PATH_CHARS) != 0)
		return (TRUE);
	else
		return (FALSE);
}

t_bool	is_relative_path(char *path, t_mshell *data)
{
	char	**splitted_path;
	int		i;

	i = -1;
	if (!path || !data)
		return (ERROR);
	if (*path == '/')
		return (FALSE);
	splitted_path = gc_split(path, '/', data->gc, 1);
	if (!splitted_path || !*splitted_path)
		return (FALSE);
	while (splitted_path[++i])
		if (ft_strcmp(splitted_path[i], ".") == 0
			|| ft_strcmp(splitted_path[i], "..") == 0
			|| (ft_strcmp(splitted_path[i], "~") == 0 && i == 0))
			return (TRUE);
	return (FALSE);
}

t_bool	is_path(char *path_start)
{
	int		i;
	t_bool	has_slash;

	i = -1;
	has_slash = FALSE;
	if (!path_start)
		return (ERROR);
	while (path_start[++i])
	{
		if (is_path_char(path_start[i]) == FALSE)
			break ;
		if (path_start[i] == '/')
			has_slash = TRUE;
	}
	return (has_slash);
}
