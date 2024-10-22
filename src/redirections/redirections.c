/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:30:13 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/22 16:46:51 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_redirs	*create_r_node(char **r_token, char **tokens,
		t_mshell *data)
{
	int				cmd_id;
	int				fd;
	t_redir_type	type;
	char			**split;
	t_redirs		*new;

	if (!r_token || !tokens || !data)
		return (NULL);
	split = gc_split(*r_token, *R_S_SEP, data->gc, 1);
	if (!split || !*split)
		return (NULL);
	if (is_single_cmd(tokens) == TRUE)
		cmd_id = 0;
	else
		cmd_id = r_id_by_reference(r_token, tokens);
	type = read_redirection_type(split[0]);
	// fd = try_open(split[1], type, data);
	fd = -2;
	new = new_redirection(&fd, &cmd_id, &type, data);
	new->errcorde = errno;
	new->path = gc_strdup(read_quoting(split[1], data), data->gc, 1);
	if (!new->path)
		return (NULL);
	return (new);
}

t_redirs	*build_red_list(char **red_tokens, char *reference, t_mshell *data)
{
	int			i;
	t_redirs	*list;

	if (!red_tokens || !reference || !data)
		return (NULL);
	i = -1;
	list = NULL;
	while (red_tokens[++i])
	{
		if (has_redir(red_tokens[i]))
		{
			if (!list)
				list = create_r_node(&red_tokens[i], red_tokens, data);
			else
				get_last_redir(&list)->next
					= create_r_node(&red_tokens[i], red_tokens, data);
			if (!list)
				return (NULL);
		}
	}
	return (list);
}

char	**split_redirections(char *input, t_mshell *data)
{
	char	**redir_tokens;

	if (!input || !data)
		return (NULL);
	redir_tokens = gc_split(input, *RED_SEP, data->gc, 1);
	if (!redir_tokens || !*redir_tokens)
		return (NULL);
	if (check_red_syntax(redir_tokens, input, data) == FALSE)
		return (NULL);
	return (redir_tokens);
}

t_redirs	*extract_redirections(t_parse *parse, t_mshell *data)
{
	t_redirs	*list;
	char		**redir_tokens;

	if (!parse->input || !data || has_redir(parse->input) == FALSE)
		return (NULL);
	parse->input = quote_closure_control(parse->input);
	parse->input = mark_redirections(parse->input, data);
	if (!parse->input)
		return (NULL);
	redir_tokens = split_redirections(parse->input, data);
	if (!redir_tokens)
		return (NULL);
	list = build_red_list(redir_tokens, parse->input, data);
	if (!list)
		return (NULL);
	parse->input = rm_redirections(parse->input);
	// print_redirection_list(list, data);
	return (list);
}
