/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 18:24:41 by estegana          #+#    #+#             */
/*   Updated: 2024/10/22 19:34:02 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exit_selection(t_cmd *cmd)
{
	if (!cmd)
		return (-5);
	if (access(cmd->args[0], F_OK))
	{
		custom_shell_error(cmd->args[0], "No such file or directory\0", 127, cmd->link);
		return (127);
	}
	if (access(cmd->args[0], X_OK))
	{
		custom_shell_error(cmd->args[0], "Permission denied\0", 126, cmd->link);
		return (126);
	}
	return (0);
}

int	ft_execute(t_cmd *list)
{
	int	excode;

	excode = 0;
	if (execve(list->args[0], list->args, list->env) == -1)
	{
		excode = exit_selection(list);
		clear_data(list->link);
		exit(excode);
	}
	return (0);
}
