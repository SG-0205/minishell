/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:43:51 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/20 15:28:41 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

t_bool	init_data(t_mshell *data, char **env)
{
	if (!data)
		return (FALSE);
	data->gc = gc_init(1);
	if (!data->gc)
		return (FALSE);
	if (build_var_list(env, data) == ENV_ERROR)
		return (FALSE);
	if (signal_handlers_setup(data) == FALSE)
		return (FALSE);
	return (TRUE);
}

void	clear_data(t_mshell *data)
{
	if (!data)
		return ;
	gc_flush(data->gc);
	free(data);
}

int	main(int argc, char **argv, char **env)
{
	char		*buffer;
	t_mshell	*data;

	if (argc != 1)
		return (1);
	(void)argv;
	data = (t_mshell *)malloc(sizeof(t_mshell));
	if (!data || init_data(data, env) == FALSE)
		return (1);
	builtin_error("cd", "te_st", EACCES, data);
	while (1)
	{
		buffer = readline(GREEN ITALIC "minishell$ " RESET);
		if (empty_line(buffer) == FALSE)
			add_history(buffer);
		if (!buffer)
		{
			printf("EOF\n");
			break ;
		}
		printf("%s\tNB_CMD = %d\n", buffer, count_cmds(buffer));
	}
	print_env(data);
	clear_data(data);
	free(buffer);
	return (0);
}
