/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:30:13 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/30 13:26:22 by sgoldenb         ###   ########.fr       */
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
			input[i] = ' ';
		}
	}
	return (input);
}

t_bool	is_single_cmd(char **input)
{
	int	i;
	int	j;

	if (!input)
		return (ERROR);
	i = -1;
	while (input[++i])
	{
		j = -1;
		while (input[i][++j])
			if (input[i][j] == '|'
				&& is_quoted_by('\'', &input[i][j], input[i]) == FALSE
				&& is_quoted_by('\"', &input[i][j], input[i]) == FALSE)
				return (FALSE);
	}
	return (TRUE);
}

int	r_id_by_reference(char **r_token, char **ref)
{
	int		id;
	t_bool	passed;
	int		i;
	int		j;

	i = -1;
	id = 0;
	passed = FALSE;
	while (ref[++i])
	{
		if (&ref[i] == r_token)
			passed = TRUE;
		j = -1;
		while (ref[i][++j])
			if (ref[i][j] == '|'
				&& is_quoted_by('\'', &ref[i][j], ref[i]) == FALSE
				&& is_quoted_by('\"', &ref[i][j], ref[i]) == FALSE)
				{
					id ++;
					if (passed == TRUE)
						return (id - 1);
				}
	}
	return (id);
}

int	try_open(char *path, t_redir_type type, t_mshell *data)
{
	char	*complete_path;
	int		fd;

	if (!path || !data || type < 0)
		return (-2);
	fd = -5;
	if (is_relative_path(path, data) == TRUE)
		complete_path = extend_path(path, data);
	else
		complete_path = path;
	if (type == HEREDOC)
		fd = heredoc_fd(complete_path, data);
	else if (type == APPEND)
		fd = open(complete_path, O_CREAT | O_APPEND | O_WRONLY,
			S_IRUSR | S_IWUSR);
	else if (type == OUTPUT)
		fd = open(complete_path, O_CREAT | O_WRONLY | O_TRUNC,
			S_IRUSR | S_IWUSR);
	else if (type == INPUT)
		fd = open(complete_path, O_RDONLY, S_IRUSR);
	if (type != HEREDOC && fd < 0)
		mshell_error(path, errno, data);
	return (fd);
}

int	is_not_operator(char c)
{
	if (ft_cisar(c, "<>|") == 1)
		return (0);
	else
		return (1);
}

char	*next_operator(char **red_split, t_mshell *data)
{
	int	i;
	int	j;

	if (!red_split || !data)
		return (NULL);
	i = 0;
	while (red_split[++i])
	{
		j = -1;
		while (red_split[i][++j])
		{
			if (ft_cisar(red_split[i][j], "<>|") == 1
				&& is_quoted_by('\'', &red_split[i][j], red_split[i]) == FALSE
				&& is_quoted_by('\"', &red_split[i][j], red_split[i]) == FALSE)
				return (gc_strndup(&red_split[i][j], ft_strblen(&red_split[i][j],
					is_not_operator), data->gc, 1));
		} 
	}
	return (NULL);
}

t_bool	check_red_syntax(char **red_tokens, char *ref, t_mshell *data)
{
	int		i;
	char	**red_split;

	if (!red_tokens || !ref)
		return (FALSE);
	i = -1;
	while (red_tokens[++i])
	{
		printf("TOKEN[%d] %s\n", i, red_tokens[i]);
		if (has_redir(red_tokens[i]) == TRUE)
		{
			red_split = gc_split(red_tokens[i], *R_S_SEP, data->gc, 1);
			printf("\t%s + %s\n", red_split[0], red_split[1]);
			if (!red_split[1])
			{
				syntax_error(next_operator(&red_tokens[i], data), data);
				return (FALSE);
			}
		}
	}
	return (TRUE);
}

t_redirs	*create_r_node(t_redirs *start, char **r_token,
	char **tokens, t_mshell *data)
{
	int				cmd_id;
	int				fd;
	t_redir_type	type;
	char			**split;

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
	fd = try_open(split[1], type, data);
	if (fd < 0)
		return (NULL);
	if (!start)
		return (new_redirection(&fd, &cmd_id, &type, data));
	get_last_redir(&start)->next = new_redirection(&fd, &cmd_id, &type, data);
	return (start);
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
		if (has_redir(red_tokens[i]))
		{
			list = create_r_node(list, &red_tokens[i], red_tokens, data);
			if (!list)
				return (NULL);
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
	printf("\nCLEAN_INPUT [%s]\n", parse->input);
	return (list);	
}
