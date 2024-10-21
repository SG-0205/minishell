/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multicheck.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:53:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/08 12:54:12 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_f_check	errored_object(void)
{
	t_f_check	error;

	error.exec = ERROR;
	error.exists = ERROR;
	error.read = ERROR;
	error.write = ERROR;
	error.is_dir = ERROR;
	error.is_file = ERROR;
	return (error);
}

static t_f_check	init_obj(t_f_check *obj)
{
	if (!obj)
		return (errored_object());
	obj->exec = ERROR;
	obj->exists = ERROR;
	obj->read = ERROR;
	obj->write = ERROR;
	obj->is_dir = ERROR;
	obj->is_file = ERROR;
	return (*obj);
}

static void	check_path_stats(char *path, t_f_check *obj)
{
	if (!obj)
		return ;
	if (stat(path, &obj->path_stats) != 0)
		return ;
	if (S_ISREG(obj->path_stats.st_mode))
		obj->is_file = TRUE;
	else if (S_ISDIR(obj->path_stats.st_mode))
		obj->is_dir = TRUE;
}

static void	check_fd_stats(int *fd, t_f_check *obj)
{
	if (!obj)
		return ;
	if (fstat(*fd, &obj->fd_stats) < 0)
		obj->exists = FALSE;
	else
		obj->exists = TRUE;
	if (obj->fd_stats.st_mode & S_IRUSR)
		obj->read = TRUE;
	if (obj->fd_stats.st_mode & S_IWUSR)
		obj->write = TRUE;
	if (obj->fd_stats.st_mode & S_IXUSR)
		obj->exec = TRUE;
	if (S_ISDIR(obj->fd_stats.st_mode))
		obj->is_dir = TRUE;
	if (S_ISREG(obj->fd_stats.st_mode))
		obj->is_file = TRUE;
}

t_f_check	f_access_check(char *path, int *fd)
{
	t_f_check	checks;

	checks = init_obj(&checks);
	if (!fd)
	{
		if (access(path, F_OK) == 0)
			checks.exists = TRUE;
		if (checks.exists == FALSE)
			return (checks);
		if (access(path, R_OK) == 0)
			checks.read = TRUE;
		if (access(path, W_OK) == 0)
			checks.write = TRUE;
		if (access(path, X_OK) == 0)
			checks.exec = TRUE;
		check_path_stats(path, &checks);
	}
	else if (*fd >= 0)
		check_fd_stats(fd, &checks);
	return (checks);
}
