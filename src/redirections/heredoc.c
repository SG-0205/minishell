/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:08:11 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/25 20:01:02 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_bool	check_bad_limiter(char *limiter, t_mshell *data)
{
	int	i;

	if (*limiter == 0)
	{
		syntax_error("syntax error near unexpected token `newline\'", 129, data);
		return (FALSE);
	}
	i = -1;
	while (limiter[++i])
		if (ft_cisar(limiter[i], "<>|") == 1)
		{
			//build error str;
			return (FALSE);
		}
	return (TRUE);
}

void	*bad_eof(int ernum, t_mshell *data, int fd)
{
	
	syntax_error("syntax error near unexpected token `newline\'", ernum, data);
	(void)fd;
	//build error str;
	return (NULL);
}

char	*rm_limiter_quoting(char *limiter)
{
	int	i;

	if (!limiter)
		return (NULL);
	i = -1;
	while (limiter[++i])
		if ((limiter[i] == '\'' || limiter[i] == '\"')
			&& is_quoted_by('\'', &limiter[i], limiter) == FALSE
			&& is_quoted_by('\"', &limiter[i], limiter) == FALSE)
		{
			str_shrink(&limiter[i]);
		}
	return (limiter);
}

t_hd_l_type	get_hd_output_type(char *limiter)
{
	t_hd_l_type	type;
	int			i;

	limiter = quote_closure_control(limiter);
	printf("[%s] LIMITER @ TYPE1\n", limiter);
	if (*limiter == '-')
		type = UNTAB;
	else
		type = NORMAL;
	i = -1;
	while (limiter[++i])
	{
		if (limiter[i] == '\'' && type == UNTAB)
			type = UNTAB_SQ;
		else if (limiter[i] == '\'' && type == NORMAL)
			type = SQ;
	}
	limiter = rm_limiter_quoting(limiter);
	printf("[%s] LIMITER @ TYPE2\n", limiter);
	limiter = revert_unclosed(limiter);
	return (type);
}

char	*rm_tabulations(char *final)
{
	int	i;

	if (!final)
		return (NULL);
	i = -1;
	while (final[++i])
		if (final[i] == '\t')
			str_shrink(&final[i]);
	return (final);
}

char	*curate_output(t_hd_l_type type, char *final, t_mshell *data)
{
	if (!final || !data)
		return (NULL);
	if (type == SQ)
		return (final);
	else if (type == NORMAL)
		return (read_quoting(final, data));
	else if (type == UNTAB)
		return (read_quoting(rm_tabulations(final), data));
	else if (type == UNTAB_SQ)
		return (rm_tabulations(final));
	else
		return (NULL);
}

char	*heredoc(char *limiter, t_hd_l_type type, int fd, t_mshell *data)
{
	char	*rl_buff;
	char	*line_buff;
	char	*final;

	if (check_bad_limiter(limiter, data) == FALSE)
		return (NULL);
	printf("LIMITER = |%s - %d|\n", limiter, *limiter);
	final = gc_strnew(1, data->gc, 1);
	while (1)
	{
		rl_buff = readline("> ");
		if (!rl_buff)
			return (bad_eof(129, data, fd));
		gc_add_ref(data->gc, rl_buff, 1);
		if (ft_strcmp(rl_buff, limiter) == 0)
			break ;
		line_buff = gc_strnew(ft_strlen(rl_buff) + 1, data->gc, 1);
		line_buff = ft_strcat(line_buff, gc_strjoin(rl_buff, "\n", data->gc, 1));
		final = gc_strjoin(final, line_buff, data->gc, 1);
	}
	final[ft_strlen(final) - 1] = 0;
	return (curate_output(type, final, data));
}

int	heredoc_fd(char *raw_limiter, t_mshell *data)
{
	char		*content;
	int			fd;
	t_hd_l_type	type;

	if (!raw_limiter || !data)
		return (-2);
	fd = open(HEREDOC_PATH, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd < 0)
		return (fd);
	printf("[%s] RAW_LIMITER\n", raw_limiter);
	printf("[%d] FD\n", fd);
	type = get_hd_output_type(raw_limiter);
	printf("[%d] TYPE\n", type);
	content = heredoc(raw_limiter, type, fd, data);
	printf("[%s] CONTENT\n", content);
	write(fd, content, ft_strlen(content));
	return (fd);
}
