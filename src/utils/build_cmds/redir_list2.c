/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_list2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 07:51:54 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/18 13:27:59 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_bool	has_redirs_by_id(int *cmd_id, t_redirs *redirs)
{
	t_redirs	*search;

	if (!cmd_id || !redirs)
		return (FALSE);
	search = get_last_redir_by_cmd_id(&redirs, *cmd_id);
	if (!search)
		return (FALSE);
	return (TRUE);
}

t_redirs	*add_path(t_redirs *redir, char *path, t_mshell *data)
{
	if (!data || !data || !redir)
		return (NULL);
	redir->path = gc_strdup(path, data->gc, 1);
	return (redir);
}
