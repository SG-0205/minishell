/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:43:51 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/26 16:09:46 by sgoldenb         ###   ########.fr       */
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

static void	update_return_code(int exit_code, t_mshell *data)
{
	if (!data || !data->env)
		return ;
	update_var(data, "$?", gc_itoa(exit_code, data->gc, 1));
}

void	builtins_test(char **expanded_input, t_mshell *data)
{
	int	exit_code;

	exit_code = 0;
	if (ft_strcmp(*expanded_input, "export") == 0)
		exit_code = export(data, &expanded_input[1]);
	else if (ft_strcmp(*expanded_input, "cd") == 0)
		exit_code = cd(&expanded_input[1], data);
	else if (ft_strcmp(*expanded_input, "env") == 0)
		exit_code = print_env(data, PUBLIC_VARS);
	else if (ft_strcmp(*expanded_input, "unset") == 0)
		exit_code = unset(&expanded_input[1], data);
	else if (ft_strcmp(*expanded_input, "exit") == 0)
		exit_b(&expanded_input[1], data);
	else if (ft_strcmp(*expanded_input, "pwd") == 0)
		exit_code = pwd(&expanded_input[1], data);
	else if (ft_strcmp(*expanded_input, "echo") == 0)
		exit_code = echo_b(&expanded_input[1], data);
	update_return_code(exit_code, data);
}

int	parse(char *input, t_mshell *data)
{
	t_parse	*parse;

	if (!input || !data)
		return (1);
	parse = (t_parse *)gc_malloc(data->gc, sizeof(t_parse), 1);
	if (!parse)
		return (-1);
	parse->input = input;
	parse->redirections = extract_redirections(input, data);
	print_redirection_list(parse->redirections);
	parse->args = initial_split(input, data);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char		*buffer;
	t_mshell	*data;
	// char		**splitted;
	// t_expand	*test;
	// char		*test2;
	// int			test_fd;

	if (argc != 1)
		return (1);
	(void)argv;
	data = (t_mshell *)malloc(sizeof(t_mshell));
	if (!data || init_data(data, env) == FALSE)
		return (1);
	printf("PID = %d\n", getpid());
	while (1)
	{
		buffer = readline(GREEN ITALIC "minishell$ " RESET);
		gc_add_ref(data->gc, buffer, 1);
		if (empty_line(buffer) == FALSE)
			add_history(buffer);
		if (!buffer)
		{
			printf("EOF\n");
			break ;
		}
		parse(buffer, data);
		// printf("|%s|\n", mark_redirections(buffer, data));
		// splitted = split_redirections(mark_redirections(buffer, data), data);
		// for (int i = 0; splitted[i]; i++)
		// 	printf("[%d]\t%s\n", i, splitted[i]);
		// heredoc_fd(buffer, data);
		// test2 = extract_content(open(HEREDOC_PATH, O_RDONLY), data);
		// printf("----BEGIN HEREDOC CONTENT----\n> %s <\n----END HEREDOC CONTENT----\n", test2);
		gc_flush_layer(data->gc, 1);
	}
	print_env(data, PUBLIC_VARS);
	clear_data(data);
	return (0);
}


//RECUPERE INPUT -> DIVISER EN COMMANDES + ARGS -> [REDIRECTIONS INPUT] -> ETENDRE LES ARGS -> EXECUTION -> [REDIRECTIONS OUTPUT]
