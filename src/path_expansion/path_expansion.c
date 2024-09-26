/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:45:45 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/26 15:35:14 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//NOUVELLE SOLUTION AVEC UNE STACK!
//NEW SOLUTION OK!

char	**split_full_path(char *path, t_mshell *data)
{
	char	**splitted_path;

	if (!data || !path)
		return (NULL);
	splitted_path = gc_split(gc_strjoinsep(get_pwd(data), path, '/', data->gc),
			'/', data->gc, 1);
	if (!splitted_path || !*splitted_path)
		return (NULL);
	return (splitted_path);
}

char	*build_path_from_list(t_pn *list, char *container)
{
	t_pn	*tmp;

	if (!container)
		return (NULL);
	if (!list)
		return (ft_strncat(container, "/", 1));
	tmp = list;
	while (tmp)
	{
		if (ft_strcmp(tmp->dir, "/") != 0)
			ft_strncat(container, "/", 1);
		ft_strncat(container, tmp->dir, ft_strlen(tmp->dir));
		// printf("\n\tCONTAINER WHEN DIR = %s\t%s\n", tmp->dir, container);
		tmp = tmp->next;
	}
	return (container);
}

char	*convert_path(char **splitted, t_mshell *data)
{
	int		i;
	t_pn	*new_p_list;
	char	*converted;

	if (!splitted || !*splitted)
		return (NULL);
	i = 0;
	new_p_list = new_path_node(splitted[0], data);
	while (splitted[++i])
	{
		if (ft_strcmp(splitted[i], ".") == 0)
			continue ;
		else if (ft_strcmp(splitted[i], "..") == 0)
			rm_last_dir(new_p_list, data);
		else
			add_dir(splitted[i], new_p_list, data);
	}
	converted = gc_strnew(full_path_len(new_p_list) + i, data->gc, 1);
	if (!converted)
		return (NULL);
	converted = build_path_from_list(new_p_list, converted);
	return (converted);
}

char	*extend_path(char *path, t_mshell *data)
{
	char	*new_path;
	char	**pwd_and_path;

	if (!path || !data)
		return (NULL);
	pwd_and_path = split_full_path(path, data);
	if (!pwd_and_path)
		return (NULL);
	new_path = convert_path(pwd_and_path, data);
	if (!new_path)
		return (NULL);
	return (new_path);
}

//SE SOUVENIR DE RENVOYER INPUT BRUT EN CAS D'ERREUR