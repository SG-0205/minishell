/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:43:51 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/03 12:38:35 by sgoldenb         ###   ########.fr       */
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
	update_var(data, "?", gc_itoa(exit_code, data->gc, 1));
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

static char	first_token(char *input)
{
	int	i;

	if (!input)
		return (0);
	i = -1;
	while (input[++i])
		if (ft_cisar(input[i], ARG_SEPARATORS) == 0
			&& is_quoted_by('\'', &input[i], input) == FALSE
			&& is_quoted_by('\"', &input[i], input) == FALSE)
			return (input[i]);
	return (0);
}

static t_bool	check_bad_pipes(char *input, t_mshell *data)
{
	if (!input || !data)
		return (FALSE);
	if (*input == '|' || first_token(input) == '|')
	{
		syntax_error("|", data);
		return (FALSE);
	}
	else if (ft_strlen(input) > 1 && input[ft_strlen(input) - 1] == '|')
	{
		syntax_error(NULL, data);
		return (FALSE);
	}
	return (TRUE);
}

int	parse(char *input, t_mshell *data)
{
	t_parse	*parse;

	if (!input || !data)
		return (1);
	if (check_bad_pipes(input, data) == FALSE)
		return (1);
	parse = (t_parse *)gc_malloc(data->gc, sizeof(t_parse), 1);
	if (!parse)
		return (-1);
	parse->input = gc_strdup(input, data->gc, 1);
	if (!parse->input)
		return (1);
	parse->redirections = extract_redirections(parse, data);
	print_redirection_list(parse->redirections, data);
	printf("INPUT after REDIRS:%s\n", parse->input);
	// parse->args = initial_split(parse->input, data);
	parse->cmds = build_cmds_list(parse, data);
	if (!parse->cmds)
		return (1);
	print_cmd_list(parse->cmds);
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
		//close_all_fds()
		gc_flush_layer(data->gc, 1);
	}
	// print_env(data, PUBLIC_VARS);
	clear_data(data);
	return (0);
}


//RECUPERE INPUT -> DIVISER EN COMMANDES + ARGS -> [REDIRECTIONS INPUT] -> ETENDRE LES ARGS -> EXECUTION -> [REDIRECTIONS OUTPUT]
