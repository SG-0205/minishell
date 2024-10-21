/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:41:35 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/09 12:22:50 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_bool	is_empty(char *token)
{
	int	i;

	if (!token)
		return (FALSE);
	i = -1;
	while (token[++i])
		if (ft_isalnum(token[i]) != 0)
			return (FALSE);
	return (TRUE);
}

t_bool	no_redirs_for_id(int *cmd_id, t_mshell *data, t_parse *parse)
{
	if (!data || !cmd_id
		|| !get_last_redir_by_cmd_id(&parse->redirections, *cmd_id))
		return (TRUE);
	return (FALSE);
}

t_bool	check_empty_cmd(char **tokens, t_parse *parsing, t_mshell *data)
{
	int	i;

	if (!tokens || !parsing || !data)
		return (FALSE);
	i = -1;
	while (tokens[++i])
	{
		if (is_empty(tokens[i]) == TRUE
			&& no_redirs_for_id(&i, data, parsing) == TRUE
			&& ft_arrlen((void **)tokens) > 1)
		{
			syntax_error("|\0", data);
			return (FALSE);
		}
	}
	return (TRUE);
}

t_bool	is_builtin(char **tokens, t_cmd *cmd, t_mshell *data)
{
	char	**builtins;
	int		i;

	if (!tokens || !*tokens || !data)
		return (ERROR);
	builtins = gc_split(BUILTINS_STR, '.', data->gc, 1);
	if (!builtins || !*builtins)
		return (ERROR);
	i = -1;
	while (builtins[++i])
	{
		if (ft_strcmp(*tokens, builtins[i]) == 0)
		{
			cmd->is_builtin = TRUE;
			return (TRUE);
		}
	}
	return (FALSE);
}
