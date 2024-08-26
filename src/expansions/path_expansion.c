/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:45:45 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 17:52:40 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_previous_dir_pwd(t_mshell *data)
{
	char	**splitted_pwd;
	char	*previous_dir_path;
	int		i;

	i = -1;
	if (!data)
		return (NULL);
	splitted_pwd = ft_split(dup_var_value(data, "PWD"), '/');
	if (!splitted_pwd)
		return (NULL);
	else if (!*splitted_pwd)
		return ("/");
	previous_dir_path = gc_strnew(1, data->gc, 0);
	if (!previous_dir_path)
		return (NULL);
	while (splitted_pwd[++i + 1])
		previous_dir_path = ft_strjoinsep(previous_dir_path, splitted_pwd[i], '/');
	ft_arrfree((void **)splitted_pwd);
	free(splitted_pwd);
	return (previous_dir_path);
}

//OUBLIE LES / DANS LES JOIN
static char	**update_splitted_path(char **splitted_path, t_mshell *data)
{
	int	i;

	i = -1;
	if (!data || !splitted_path || !*splitted_path)
		return (NULL);
	while (splitted_path[++i])
	{
		if (ft_strcmp(splitted_path[i], ".") == 0)
			splitted_path[i] = dup_var_value(data, "PWD"), str_shrink(splitted_path[i]);
		else if (ft_strcmp(splitted_path[i], "..") == 0)
			splitted_path[i] = get_previous_dir_pwd(data);
	}
	return (splitted_path);
}

char	*extend_relative_path(char *path, t_mshell *data)
{
	int		i;
	char	**splitted_path;
	char	*new_path;

	i = -1;
	if (!path)
		return (NULL);
	if (*path == '/')
		return (path);
	if (!search_var(&data->env, "PWD")->value)
		reinit_pwd(data);
	splitted_path = ft_split(path, '/');
	if (!splitted_path || !*splitted_path)
		return (NULL);
	splitted_path = update_splitted_path(splitted_path, data);
	new_path = gc_strnew(1, data->gc, 0);
	*new_path = '/';
	while (splitted_path[++i])
		new_path = ft_strjoinsep(new_path, splitted_path[i], '/');
	ft_arrfree((void **)splitted_path);
	free(splitted_path);
	str_shrink(new_path);
	return (new_path);
}
//TOTEST!!!!
//DEVRAIT GRADER LES //, A CORRIGER