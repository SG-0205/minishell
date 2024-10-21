/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 13:51:10 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/07 20:37:20 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	change_directory(char *path, t_mshell *data)
{
	char	*new_dir;
	char	*old_dir;
	char	buffer[4096];

	if (!path || !data)
		return (-1);
	if (ft_strcmp(path, "-") == 0)
	{
		new_dir = dup_var_value(data, "OLDPWD");
		printf("%s\n", new_dir);
	}
	else
		new_dir = path;
	old_dir = dup_var_value(data, "PWD");
	if (!old_dir)
		return (-1);
	if (chdir(new_dir) != 0)
		return (errno);
	update_var(data, "OLDPWD", old_dir);
	update_var(data, "PWD", getcwd(buffer, sizeof(buffer)));
	return (0);
}

int	cd(char **path, t_mshell *data)
{
	t_f_check	dir_checks;

	if (!data || !data->env)
		return (-5);
	if (!*path)
		return (change_directory(dup_var_value(data, "HOME"), data));
	if (ft_strcmp(*path, "-") == 0
		&& ft_strcmp(search_var(&data->env, "OLDPWD")->value, " ") == 0)
		return (custom_b_error("cd", "\x1C", "OLDPWD not set", data));
	else if (ft_strcmp(*path, "-") == 0)
		return (change_directory(*path, data));
	dir_checks = f_access_check(*path, NULL);
	if (ft_arrlen((void **)path) > 1)
		return (builtin_error("cd", *path, E2BIG, data));
	if (ft_strlen(*path) > 4096)
		return (builtin_error("cd", *path, ENAMETOOLONG, data));
	if (dir_checks.exists == FALSE)
		return (builtin_error("cd", *path, ENOENT, data));
	else if (dir_checks.exec == FALSE)
		return (builtin_error("cd", *path, EACCES, data));
	else if (dir_checks.is_dir == FALSE)
		return (builtin_error("cd", *path, ENOTDIR, data));
	if (change_directory(*path, data) != 0)
		return (builtin_error("cd", *path, errno, data));
	return (0);
}
