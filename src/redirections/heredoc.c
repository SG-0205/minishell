/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:08:11 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/24 15:26:27 by sgoldenb         ###   ########.fr       */
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

void	*bad_eof(int ernum, t_mshell *data)
{
	syntax_error("syntax error near unexpected token `newline\'", ernum, data);
	//build error str;
	return (NULL);
}

char	*heredoc(char *limiter, t_mshell *data)
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
			return (bad_eof(129, data));
		gc_add_ref(data->gc, rl_buff, 1);
		if (ft_strcmp(rl_buff, limiter) == 0)
			break ;
		line_buff = gc_strnew(ft_strlen(rl_buff) + 1, data->gc, 1);
		line_buff = ft_strcat(line_buff, gc_strjoin(rl_buff, "\n", data->gc, 1));
		final = gc_strjoin(final, line_buff, data->gc, 1);
	}
	final[ft_strlen(final) - 1] = 0;
	return (read_quoting(final, data));
}

char	**create_heredocs(char **args, t_mshell *data)
{
	int	i;
	int	h_count;

	if (!args || !*args || !data)
		return (NULL);
	i = -1;
	h_count = 0;
	while (args[++i])
		if (read_redirection_type(args[i]) == HEREDOC)
			h_count ++;
	i = -1;
	if (h_count == 0)
		return (args);
	else if (h_count < 2)
		args[locate_redirection(args, HEREDOC)]
			= heredoc(args[locate_redirection(args, HEREDOC)], data);
	return (args);
}
