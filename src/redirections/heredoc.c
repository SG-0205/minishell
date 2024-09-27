/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:08:11 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/27 16:22:05 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_bool	check_bad_limiter(char *limiter, t_mshell *data)
{
	int	i;

	if (*limiter == 0)
	{
		syntax_error((void *)limiter, data);
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

void	*bad_eof_hd(char *limiter, int l_count, t_mshell *data)
{

	if (!data)
		return (NULL);
	bad_eof(limiter, l_count, data);
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

static void	rm_nl(char *final)
{
	int	i;

	if (!final)
		return ;
	i = 0;
	while (final[i + 1])
		i++;
	if (final[i] == '\n')
		final[i] = 0;
}

char	*heredoc(char *limiter, t_hd_l_type type, t_mshell *data)
{
	char	*rl_buff;
	char	*line_buff;
	char	*final;
	int		line_count;

	if (check_bad_limiter(limiter, data) == FALSE)
		return (NULL);
	final = gc_strnew(1, data->gc, 1);
	line_count = 1;
	while (line_count ++)
	{
		rl_buff = readline("> ");
		if (!rl_buff)
			break ;
		gc_add_ref(data->gc, rl_buff, 1);
		if (ft_strcmp(rl_buff, limiter) == 0)
			break ;
		line_buff = gc_strnew(ft_strlen(rl_buff) + 1, data->gc, 1);
		line_buff = ft_strcat(line_buff, gc_strjoin(rl_buff, "\n", data->gc, 1));
		final = gc_strjoin(final, line_buff, data->gc, 1);
	}
	if (!rl_buff)
		bad_eof_hd(limiter, line_count, data);
	rm_nl(final);
	return (curate_output(type, final, data));
}

int	heredoc_fd(char *raw_limiter, t_mshell *data)
{
	char		*content;
	static int	hd_count;
	int			fd_w;
	t_hd_l_type	type;

	if (!raw_limiter || !data)
		return (-2);
	fd_w = open(gc_strjoin(HEREDOC_PATH, gc_itoa(hd_count, data->gc, 1),
			data->gc, 1),
		O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd_w < 0)
		return (fd_w);
	type = get_hd_output_type(raw_limiter);
	content = heredoc(raw_limiter, type, data);
	if (!content)
		return (-3);
	write(fd_w, content, ft_strlen(content));
	close(fd_w);
	hd_count ++;
	return (open(gc_strjoin(HEREDOC_PATH, gc_itoa(hd_count, data->gc, 1),
			data->gc, 1), O_RDONLY));
}
