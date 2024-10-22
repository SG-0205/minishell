/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IS_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:57:43 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/22 21:51:19 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	skip_limiter_name(char *hd_start)
{
	int		total_len;
	t_bool	name_start;

	if (!hd_start)
		return (0);
	total_len = -1;
	name_start = FALSE;
	while (hd_start[++total_len])
	{
		if ((ft_cisar(hd_start[total_len], " \t") == 1
				|| hd_start[total_len] == '<') && name_start == FALSE)
			continue ;
		if (ft_cisar(hd_start[total_len], " \t") == 1 && name_start == TRUE)
			break ;
		else
			name_start = TRUE;
	}
	return (total_len);
}

char	*get_hd_limiter(char *hd_start, t_mshell *data)
{
	int		i;
	char	*limiter;
	int		limiter_len;

	if (!hd_start || !data)
		return (NULL);
	i = 1;
	limiter_len = 0;
	while (hd_start[++i])
	{
		if (ft_cisar(hd_start[i], " \t") == 1 && limiter_len == 0)
			continue ;
		else if (ft_cisar(hd_start[i], " \t") == 1 && limiter_len > 0)
			break ;
		else
			limiter_len ++;
	}
	limiter = gc_strnew(limiter_len, data->gc, 1);
	if (!limiter)
		return (NULL);
	limiter = ft_strncpy(limiter, &hd_start[i - limiter_len], limiter_len);
	return (limiter);
}

int	cpy_redir(char *red_start, char *new)
{
	int	i;

	if (!red_start || !new)
		return (0);
	i = -1;
	new = ft_strncat(new, RED_SEP, 1);
	while (red_start[++i] && ft_cisar(red_start[i], "<>") == 1)
		new = ft_strncat(new, &red_start[i], 1);
	new = ft_strncat(new, R_S_SEP, 1);
	while (red_start[i] && ft_cisar(red_start[i], " ") == 1)
		i ++;
	return (i);
}
