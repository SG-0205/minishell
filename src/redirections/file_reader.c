/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:16:31 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/27 16:00:56 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_f_check	errored_object(void)
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
	obj->exec = FALSE;
	obj->exists = FALSE;
	obj->read = FALSE;
	obj->write = FALSE;
	obj->is_dir = FALSE;
	obj->is_file = FALSE;
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
	if (!obj || fstat(*fd, &obj->fd_stats) < 0)
		return ;
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

	if (!path)
		return (errored_object());
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

char	*extract_content(int fd, t_mshell *data)
{
	char	*file_buffer;
	char	*tmp_buffer;

	if (!data)
		return (NULL);
	printf("GNLFD[%d]\n", fd);
	file_buffer = gc_strnew(1, data->gc, 1);
	tmp_buffer = get_next_line(fd);
	while (tmp_buffer)
	{
		if (tmp_buffer)
			file_buffer = gc_strjoin(file_buffer, tmp_buffer, data->gc, 1);
		free(tmp_buffer);
		tmp_buffer = get_next_line(fd);
	}
	if (tmp_buffer)
		free(tmp_buffer);
	return (file_buffer);
}
