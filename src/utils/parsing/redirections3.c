/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:42:54 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/21 22:09:38 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

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
	return (fd);
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
			if (ft_cisar(red_split[i][j], "<>|") == 1 && is_quoted_by('\'',
					&red_split[i][j], red_split[i]) == FALSE
					&& is_quoted_by('\"', &red_split[i][j],
					red_split[i]) == FALSE)
				return (gc_strndup(&red_split[i][j],
					ft_strblen(&red_split[i][j], is_not_operator),
						data->gc, 1));
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
		if (has_redir(red_tokens[i]) == TRUE)
		{
			red_split = gc_split(red_tokens[i], *R_S_SEP, data->gc, 1);
			if (!red_split[1])
			{
				syntax_error(next_operator(&red_tokens[i], data), data);
				return (FALSE);
			}
		}
	}
	return (TRUE);
}
