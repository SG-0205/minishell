/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:03:00 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/24 09:09:40 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

//void	update_return_code(int exit_code, t_mshell *data)
//{
//	if (!data || !data->env)
//		return ;
//	update_var(data, "?", gc_itoa(exit_code, data->gc, 1));
//}

void	init_pipe_tab(t_mshell *data)
{
	int	i;

	if (!data)
		return ;
	i = -1;
	while (++i < PIPE_NBR)
	{
		data->fds[i][0] = -1;
		data->fds[i][1] = -1;
	}
}

t_envar	*init_export(t_mshell *data)
{
	t_envar	*head;

	head = gc_malloc(data->gc, sizeof(t_envar), 0);
	if (!head)
		return (NULL);
	head->name = gc_strdup("HEAD", data->gc, 0);
	if (!head->name)
		return (NULL);
	head->value = gc_strdup(head->name, data->gc, 0);
	if (!head->value)
		return (NULL);
	head->hidden = ERROR;
	head->next = NULL;
	return (head);	
}

t_bool	init_data(t_mshell *data, char **env)
{
	if (!data)
		return (FALSE);
	data->gc = gc_init(3);
	data->no_exec = FALSE;
	data->redir_link = NULL;
	data->export = init_export(data);
	init_pipe_tab(data);
	if (!data->gc)
		return (FALSE);
	if (build_var_list(env, data) == ENV_ERROR)
		return (FALSE);
	if (signal_handlers_setup(data) != TRUE)
		return (FALSE);
	return (TRUE);
}

void	bruteforce_pipes(t_mshell *data, int keep[2])
{
	int i;

	if (!data)
		return ;
	i = 2;
	while (++i < PIPE_NBR)
	{
		if (i != keep[0] && i != keep[1])
		{
			close(data->fds[i][0]);
			close(data->fds[i][1]);
		}
	}
}

void	clear_data(t_mshell *data)
{
	if (!data)
		return ;
	gc_flush(data->gc);
	free(data);
}

t_bool	empty_line(char *input)
{
	int	i;

	i = -1;
	if (!input)
		return (FALSE);
	while (input[++i])
		if (input[i] != ' ')
			return (FALSE);
	return (TRUE);
}
