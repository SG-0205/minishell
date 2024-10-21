/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:08:11 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/16 11:00:53 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//static void	rm_nl(char *final)
//{
//	int	i;

//	if (!final)
//		return ;
//	i = 0;
//	while (final[i + 1])
//		i++;
//	if (final[i] == '\n')
//		final[i] = 0;
//}

static char	*build_hd_path(t_mshell *data, int *hd_count)
{
	char	*new;

	if (!data)
		return (NULL);
	new = gc_strjoin("/sgoinfre/HD", gc_itoa(*hd_count, data->gc, 1),
			data->gc, 1);
	new = gc_strjoin(dup_var_value(data, "HOME"), new, data->gc, 1);
	if (!new)
		return ("HD");
	return (new);
}

char	*curate_output(t_hd_l_type type, char *final, t_mshell *data)
{
	if (!final || !data)
		return (NULL);
	//rm_nl(final);
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
	while (line_count++)
	{
		rl_buff = readline("> ");
		if (!rl_buff)
			break ;
		gc_add_ref(data->gc, rl_buff, 1);
		if (ft_strcmp(rl_buff, limiter) == 0)
			break ;
		line_buff = gc_strnew(ft_strlen(rl_buff) + 1, data->gc, 1);
		line_buff = ft_strcat(line_buff, gc_strjoin(rl_buff, "\n", data->gc,
					1));
		final = gc_strjoin(final, line_buff, data->gc, 1);
	}
	if (!rl_buff)
		bad_eof_hd(limiter, line_count, data);
	return (curate_output(type, final, data));
}

int	heredoc_fd(char *raw_limiter, t_mshell *data)
{
	char		*content;
	static int	hd_count;
	char		*f_name;
	int			fd_w;
	t_hd_l_type	type;

	if (!raw_limiter || !data)
		return (-2);
	f_name = build_hd_path(data, &hd_count);
	fd_w = open(f_name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd_w < 0)
		return (fd_w);
	type = get_hd_output_type(raw_limiter);
	content = heredoc(raw_limiter, type, data);
	if (!content)
		return (-3);
	write(fd_w, content, ft_strlen(content));
	close(fd_w);
	g_sig_trace = FALSE;
	hd_count++;
	return (open(f_name, O_RDONLY));
}
