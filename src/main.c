/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:43:51 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/21 16:14:25 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_sig_trace = 0;

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

t_cmd	*parse(char *input, t_mshell *data)
{
	t_parse	*parse;

	if (!input || !data)
		return (NULL);
	if (check_bad_pipes(input, data) == FALSE)
		return (NULL);
	parse = (t_parse *)gc_malloc(data->gc, sizeof(t_parse), 1);
	if (!parse)
		return (NULL);
	parse->input = gc_strdup(input, data->gc, 1);
	if (!parse->input)
		return (NULL);
	parse->redirections = extract_redirections(parse, data);
	if (!parse->redirections && has_redir(parse->input) == TRUE)
		return (NULL);
	parse->cmds = build_cmds_list(parse, data);
	if (!parse->cmds)
		return (NULL);
	return (parse->cmds);
}

void	reset_pipes(t_mshell *data)
{
	int	i;

	i = -1;
	if (!data)
		return ;
	while (++i < PIPE_NBR
		&& data->fds[i][0] != -1
		&& data->fds[i][1] != -	1)
	{
		close(data->fds[i][0]);
		close(data->fds[i][1]);
	}
	return ;
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
	if (!parse->redirections && has_redir(parse->input) == TRUE)
		return (1);
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

	if (argc != 1)
		return (1);
	data = (t_mshell *)malloc(sizeof(t_mshell));
	if (!data || init_data(data, env) == FALSE)
		return (1);
	(void)argv;
	while (1)
	{
		data->no_exec = FALSE;
		buffer = readline(GREEN ITALIC "minishell$ " RESET);
		gc_add_ref(data->gc, buffer, 1);
		if (empty_line(buffer) == FALSE)
			add_history(buffer);
		if (!buffer)
			exit_b((char *[]){"\0", NULL}, data);
		ft_exec(parse(buffer, data));
		reset_pipes(data);
		gc_flush_layer(data->gc, 1);
	}
	clear_data(data);
	return (0);
}
