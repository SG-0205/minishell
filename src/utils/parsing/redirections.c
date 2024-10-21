/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:36:30 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/24 15:33:33 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_bool	compact_heredoc(char *arg)
{
	int	i;

	if (!arg)
		return (FALSE);
	i = -1;
	while (arg[++i])
		if (arg[i + 1] && (arg[i] == '<' && arg[i + 1] == '<'))
			return (TRUE);
	return (FALSE);
}

t_bool	is_a_redirection(char *arg)
{
	if (!arg)
		return (FALSE);
	if (ft_strcmp(arg, "<") == 0 || (ft_strcmp(arg, "<<") == 0
			|| compact_heredoc(arg) == TRUE) || ft_strcmp(arg, ">") == 0
		|| ft_strcmp(arg, ">>") == 0)
		return (TRUE);
	return (FALSE);
}

t_redir_type	read_redirection_type(char *arg)
{
	if (!arg || is_a_redirection(arg) == FALSE)
		return (-1);
	if (ft_strcmp(arg, ">") == 0)
		return (OUTPUT);
	else if (ft_strcmp(arg, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(arg, "<") == 0)
		return (INPUT);
	else if (ft_strcmp(arg, "<<") == 0 || compact_heredoc(arg) == TRUE)
		return (HEREDOC);
	else
		return (-1);
}

int	locate_redirection(char **args, t_redir_type redir_type)
{
	int	i;

	if (!args || !*args)
		return (-1);
	i = -1;
	while (args[++i])
		if (read_redirection_type(args[i]) == redir_type)
			return (i);
	return (-1);
}
