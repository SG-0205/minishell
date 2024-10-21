/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:16:31 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/21 16:14:25 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*extract_content(int fd, t_mshell *data)
{
	char	*file_buffer;
	char	*tmp_buffer;

	if (!data)
		return (NULL);
//
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
