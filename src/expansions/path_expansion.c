/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:45:45 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 15:58:18 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_previous_dir_pwd(t_mshell *data)
{
	char	**splitted_pwd;
	char	*previous_dir_path;
	int		i;

	i = 0;
	if (!data)
		return (NULL);
	splitted_pwd = gc_split(dup_var_value(data, "PWD"), '/', data->gc, 0);
	if (!splitted_pwd)
		return (NULL);
	else if (!*splitted_pwd)
		return ("/");
	previous_dir_path = gc_strjoin("/", gc_strdup(splitted_pwd[i], data->gc, 0),
			data->gc, 0);
	if (!previous_dir_path)
		return (NULL);
	while (splitted_pwd[++i + 1])
		gc_strjoin(previous_dir_path, gc_strjoin("/", splitted_pwd[i], data->gc,
				0), data->gc, 0);
	return (previous_dir_path);
}

//OUBLIE LES / DANS LES JOIN
static void	**update_splitted_path(char **splitted_path, t_mshell *data)
{
	int	i;

	i = 0;
	if (!data || !splitted_path || !*splitted_path)
		return (NULL);
	while (splitted_path[++i])
	{
		if (ft_strcmp(splitted_path[i], ".") == 0)
			splitted_path[i] = dup_var_value(data, "PWD");
		else if (ft_strcmp(splitted_path[i], "..") == 0)
			splitted_path[i] = get_previous_dir_pwd(data);
	}
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
	if (!search_var(&data->env, "PWD"))
		reinit_pwd(data);
	splitted_path = ft_split(path, '/');
	if (!splitted_path || !*splitted_path)
		return (NULL);
	update_splitted_path(splitted_path, data);
	new_path = gc_strjoin("/", splitted_path[i], data->gc, 0);
	while (splitted_path[++i])
		gc_strjoin(splitted_path, gc_strjoin("/", splitted_pwd[i], data->gc,
				0), data->gc, 0);
	return (NULL);
}
//TOTEST!!!!
//DEVRAIT GRADER LES //, A CORRIGER