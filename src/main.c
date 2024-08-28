/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:43:51 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/28 16:47:51 by sgoldenb         ###   ########.fr       */
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
	data->gc = gc_init(2);
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
	// t_expand	*test;
	char		**test2;

	if (argc != 1)
		return (1);
	(void)argv;
	data = (t_mshell *)malloc(sizeof(t_mshell));
	if (!data || init_data(data, env) == FALSE)
		return (1);
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
		test2 = initial_split(buffer, data);
		if (!test2 || !*test2)
			printf("KO\n");
		else
			for (int i = 0; test2[i]; i++)
				printf("ARGS[%d]:\t%s\n", i, test2[i]);
		// test = new_expansion(buffer, data);
		// print_exp(test, "MAIN");
		// gc_flush_layer(data->gc, 1);
		// printf("%s is a path = %d\n", buffer, is_path(buffer));
		// printf("%s is a r_path = %d\n", buffer, is_relative_path(buffer, data));
		// printf("\nDUPVAR:\t%s\n", dup_var_value(data, "PWD"));
		// printf("\n\nPATH:\t%s\nMOD_PATH:\t%s\n", buffer, extend_relative_path(buffer, data));
		// env_list_to_array(data, FALSE);
	}
	print_env(data);
	clear_data(data);
	free(buffer);
	return (0);
}


//RECUPERE INPUT -> DIVISER EN COMMANDES + ARGS -> [REDIRECTIONS INPUT] -> ETENDRE LES ARGS -> EXECUTION -> [REDIRECTIONS OUTPUT]