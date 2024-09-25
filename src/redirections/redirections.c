/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:30:13 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/25 00:58:03 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_bool	has_redir(char *input)
{
	int	i;

	if (!input)
		return (FALSE);
	i = -1;
	while (input[++i])
	{
		if (ft_cisar(input[i], "<>") == 1
			&& is_quoted_by('\'', &input[i], input) == FALSE
			&& is_quoted_by('\"', &input[i], input) == FALSE)
			return (TRUE);
	}
	return (FALSE);
}

char	*rm_redirections(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
	{
		if (input[i] == *RED_SEP)
		{
			input[i] = ' ';
			while (input[i++] && input[i] != *RED_SEP)
				input[i] = ' ';
		}
	}
	return (input);
}

char	**split_redirections(char *input, t_mshell *data)
{
	int		i;
	char	**redir_tokens;

	if (!input || !data)
		return (NULL);
	redir_tokens = gc_split(input, *RED_SEP, data->gc, 1);
	if (!redir_tokens || !*redir_tokens)
		return (NULL);
	i = -1;
	while (redir_tokens[++i])
		if (ft_cisar(*redir_tokens[i], "<>") == 0)
			ft_bzero(redir_tokens[i], ft_strlen(redir_tokens[i]));
	return (redir_tokens);
}

t_redirs	*build_red_list(char **red_tokens, char *reference, t_mshell *data)
{
	int			i_t;
	t_redirs	*list;

	if (!red_tokens || !reference || !data)
		return (NULL);
	i_t = -1;
	while (red_tokens[++i_t])
	{
		if (*red_tokens[i_t])
	}
}

t_redirs	*extract_redirections(char *input, t_mshell *data)
{
	t_redirs	*list;
	char		**redir_tokens;

	if (!input || !data || has_redirs(input) == FALSE)
		return (NULL);
	input = quote_closure_control(input);
	input = mark_redirections(input, data);
	if (!input)
		return (NULL);
	redir_tokens = split_redirections(input, data);
	if (!redir_tokens)
		return (NULL);
	list = build_red_list(redir_tokens, input, data);
	input = rm_redirections(input);
	return (list);	
}
