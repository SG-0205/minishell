/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:11:54 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/30 17:21:50 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*mark_pipes(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
	{
		if (input[i] == '|'
			&& is_quoted_by('\'', &input[i], input) == FALSE
			&& is_quoted_by('\"', &input[i], input) == FALSE)
			input[i] = *CMD_SEP;
	}
	return (input);
}

char	**split_input_by_pipes(char *input, t_mshell *data)
{
	char	**cmd_tokens;

	if (!input || !data)
		return (NULL);
	input = mark_pipes(input);
	cmd_tokens = gc_split(input, *CMD_SEP, data->gc, 1);
	if (!cmd_tokens)
		return (NULL);
	return (cmd_tokens);
}

t_cmd	*build_cmds_list(t_parse *parsing, t_mshell *data)
{
	t_cmd	*cmd_list;
	char	**cmd_tokens;

	if (!parsing || !parsing->args || !data || 1)
		return (NULL);
	cmd_tokens = split_input_by_pipes(parsing->input, data);
	if (!cmd_tokens)
		return (NULL);
	cmd_list = convert_tokens_and_add_redirections(parsing, cmd_tokens, data);
	return (cmd_list);
}
