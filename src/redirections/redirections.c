/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:30:13 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/26 17:11:29 by sgoldenb         ###   ########.fr       */
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

t_bool	is_single_cmd(char *input)
{
	int	i;

	if (!input)
		return (ERROR);
	i = -1;
	while (input[++i])
	{
		if (input[i] == '|'
			&& is_quoted_by('\'', &input[i], input) == FALSE
			&& is_quoted_by('\"', &input[i], input) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

int	r_id_by_reference(char *r_token, char *ref)
{
	int	id;
	int	i;

	if (!r_token || !ref)
		return (-1);
	else if (is_single_cmd(ref) == TRUE)
		return (0);
	i = -1;
	id = 0;
	while (ref[++i])
	{
		if (ref[i] == '|'
			&& is_quoted_by('\'', &ref[i], ref) == FALSE
			&& is_quoted_by('\"', &ref[i], ref) == FALSE)
		{
			id ++;
			if (ft_strstr(ref, r_token) < &ref[i])
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
		fd = open(complete_path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
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

char	*next_operator(char *red_token, char *ref, t_mshell *data)
{
	int	i;

	if (!red_token || !ref || !data)
		return (NULL);
	i = -1;
	printf("REF = %s\tTOKEN = %s\n", ref, red_token);
	while (ref[++i])
	{
		if (ft_strncmp(red_token, &ref[i], ft_strlen(red_token)) == 0)
		{
			printf("OK\n");
			i += ft_strlen(red_token);
			while (ref[i])
			{
				if (ft_cisar(ref[i], "<>|") == 1
					&& is_quoted_by('\'', &ref[i], ref) == FALSE
					&& is_quoted_by('\"', &ref[i], ref) == FALSE)
					return (gc_strndup(&ref[i], ft_strblen(&ref[i],
						is_not_operator), data->gc, 1));
				i ++;
			} 
		}
	}
	return (NULL);
}

t_bool	check_red_syntax(char **red_tokens, char *ref, t_mshell *data)
{
	int	i;
	// int	j;

	if (!red_tokens || !ref)
		return (FALSE);
	i = -1;
	while (red_tokens[++i])
	{
		printf("TOKEN[%d] %s\n", i, red_tokens[i]);
		if (*red_tokens[i] && !gc_split(red_tokens[i], *R_S_SEP, data->gc, 1)[1])
		{
			printf("OK\n");
			syntax_error(next_operator(red_tokens[i], ref, data), data);
			return (FALSE);
		}
	}
	return (TRUE);
}

t_redirs	*create_r_node(t_redirs *start, char *r_token,
	char *ref, t_mshell *data)
{
	int				cmd_id;
	int				fd;
	t_redir_type	type;
	char			**split;

	if (!r_token || !ref || !data)
		return (NULL);
	split = gc_split(r_token, *R_S_SEP, data->gc, 1);
	if (!split || !split)
		return (NULL);
	cmd_id = r_id_by_reference(r_token, ref);
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
	int			i_t;
	t_redirs	*list;

	if (!red_tokens || !reference || !data)
		return (NULL);
	i_t = -1;
	list = NULL;
	while (red_tokens[++i_t])
		if (*red_tokens[i_t])
		{
			list = create_r_node(list, red_tokens[i_t], reference, data);
			if (!list)
				return (NULL);
		}
	return (list);
}

t_redirs	*extract_redirections(char *input, t_mshell *data)
{
	t_redirs	*list;
	char		**redir_tokens;

	if (!input || !data || has_redir(input) == FALSE)
		return (NULL);
	input = quote_closure_control(input);
	input = mark_redirections(input, data);
	if (!input)
		return (NULL);
	redir_tokens = split_redirections(input, data);
	if (!redir_tokens)
		return (NULL);
	if (check_red_syntax(redir_tokens, input, data) == FALSE)
		return (NULL);
	list = build_red_list(redir_tokens, input, data);
	if (!list)
		return (NULL);
	input = rm_redirections(input);
	return (list);	
}
