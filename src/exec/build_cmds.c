/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:11:54 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/02 17:43:38 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*mark_pipes(char *input)
{
	int	i;

	if (!input)
		return (NULL);
	i = -1;
	while (input[++i])
	{
		if (input[i] == '|'
			&& is_quoted_by('\'', &input[i], input) == FALSE
			&& is_quoted_by('\"', &input[i], input) == FALSE)
			input[i] = *CMD_SEP;
	}
	printf(RED "\n\n%s\n\n" RESET, input);
	return (input);
}

char	**split_input_by_pipes(char *input, t_mshell *data)
{
	char	**cmd_tokens;

	if (!input || !data)
		return (NULL);
	input = mark_pipes(input); 
	cmd_tokens = gc_split(input, *CMD_SEP, data->gc, 1);
	if (!cmd_tokens)
		return (NULL);
	return (cmd_tokens);
}

char	*path_search(char *to_search, t_mshell *data)
{
	int		i;
	char	**path_dirs;
	t_envar	*path_search_try;

	if (!to_search || !data)
		return (NULL);
	path_search_try = search_var(&data->env, "PATH");
	if (!path_search_try)
		return (to_search);
	path_dirs = gc_split(path_search_try->value, ':', data->gc, 1);
	if (!path_dirs)
		return (to_search);
	i = -1;
	while (path_dirs[++i])
		if (access(gc_strjoin(gc_strjoin(path_dirs[i], "/\0", data->gc, 1),
			to_search, data->gc, 1), X_OK) == 0)
			return ((gc_strjoin(gc_strjoin(path_dirs[i], "/\0", data->gc, 1),
				to_search, data->gc, 1)));
	return (NULL);
}

char	*convert_rp(char *relative_path, t_mshell *data)
{
	char		*converted;
	t_f_check	checks;

	if (!relative_path || !data)
		return (NULL);
	converted = extend_path(relative_path, data);
	if (!converted)
		return (NULL);
	checks = f_access_check(relative_path, NULL);
	if (checks.exists == FALSE)
	{
		custom_shell_error(relative_path, strerror(EACCES), 127, data);
		return (NULL);
	}
	else if (checks.is_dir == TRUE)
	{
		custom_shell_error(relative_path, strerror(EISDIR), 126, data);
		return (NULL);
	}
	else if (checks.exec == FALSE)
	{
		custom_shell_error(relative_path, strerror(EACCES), 126, data);
		return (NULL);
	}
	return (converted);
}

char	*interpolate_path(char *cmd_name, t_mshell *data)
{
	char		*new;
	t_f_check	checks;

	if (!cmd_name || !data || !*cmd_name)
		return (NULL);
	else if (is_relative_path(cmd_name, data) == TRUE)
		return (convert_rp(cmd_name, data));
	new = path_search(cmd_name, data);
	if (!new)
	{
		custom_shell_error(cmd_name, "command not found\0", 127, data);
		return (NULL);
	}
	checks = f_access_check(new, NULL);
	if (checks.is_dir == TRUE)
	{
		custom_shell_error(cmd_name, strerror(EISDIR), 126, data);
		return (NULL);
	}
	else if (checks.exec == FALSE)
	{
		custom_shell_error(cmd_name, strerror(EACCES), 126, data);
		return (NULL);
	}
	return (new);
}

char	**dup_cmd_args(char	**tokens, t_mshell *data)
{
	int		i;
	int		args_size;
	char	**new;

	if (!tokens || !*tokens || !data)
		return (NULL);
	i = -1;
	args_size = ft_arrlen((void **)tokens);
	new = (char **)gc_malloc(data->gc, sizeof(char *) * (args_size + 1), 1);
	if (!new)
		return (NULL);
	new[args_size] = NULL;
	while (++i < args_size)
		new[i] = gc_strdup(tokens[i], data->gc, 1);
	return (new);	
}

t_cmd	*select_tokens_and_pass_env(t_cmd *cmd, char **tokens, t_mshell *data)
{
	if (!cmd || !data)
		return (NULL);
	if (tokens)
	{
		if (tokens[0])
			cmd->path_to_cmd = interpolate_path(*tokens, data);
		if (tokens[0] && !cmd->path_to_cmd)
			return (NULL);
		if (tokens[1])
			cmd->args = dup_cmd_args(&tokens[1], data);
	}
	cmd->env = env_list_to_array(data, FALSE);
	return (cmd);
}

static int	apply_redirection(t_redirs *redirections, t_redir_type i_o,
	int *cmd_id, t_mshell *data)
{
	t_redirs	*filtered_list;
	t_redirs	*final_item;

	if (!redirections || !data || (i_o != INPUT && i_o != OUTPUT))
		return (-1);
	filtered_list = filter_redirs_by_type(&redirections, i_o, data);
	printf("FILTERED FOR %d\t%p\n", *cmd_id, filtered_list);
	if (!filtered_list)
		return (-1);
	final_item = get_last_redir_by_cmd_id(&filtered_list, *cmd_id);
	if (!final_item)
		return (-1);
	printf("FINAL ITEM FOR %d\t%p\n", *cmd_id, final_item);
	return (final_item->fd);
}

t_cmd	*apply_redirections(t_cmd *cmd, t_parse *parsing,
	int *cmd_id, t_mshell *data)
{
	if (!parsing || !data || !cmd)
		return (NULL);
	else if (!parsing->redirections)
		return (cmd);
	cmd->input_fd = apply_redirection(parsing->redirections, INPUT,
		cmd_id, data);
	cmd->output_fd = apply_redirection(parsing->redirections, OUTPUT,
		cmd_id, data);
	if (cmd->input_fd < 2)
		cmd->input_fd = 0;
	if (cmd->output_fd < 2)
		cmd->output_fd = 0;
	return (cmd);
}

t_cmd	*new_cmd_from_tokens(char *tokens, t_parse *parsing, t_mshell *data,
	int *cmd_id)
{
	t_cmd	*new_cmd;
	char	**splitted_cmds;

	if (!tokens || !parsing || !data)
		return (NULL);
	//TODO Verifier la commande vide;
	new_cmd = new_empty_cmd(data);
	if (!new_cmd)
		return (NULL);
	splitted_cmds = initial_split(tokens, data);
	printf("NEW EMPTY OK\n");
	new_cmd = select_tokens_and_pass_env(new_cmd, splitted_cmds, data);
	if (!new_cmd)
		return (NULL);
	printf("ENV + ARGS OK\n");
	new_cmd = apply_redirections(new_cmd, parsing, cmd_id, data);
	if (!new_cmd)
		return (NULL);
	printf("REDIRS OK\n");
	return (new_cmd);
}

t_cmd	*link_input_pipes(t_cmd **cmds, t_mshell *data)
{
	t_cmd	*prev;
	t_cmd	*tmp;
	int		cmd_id;
	int		lst_size;

	if (!cmds || !*cmds || !data)
		return (NULL);
	lst_size = cmd_list_size(cmds);
	cmd_id = -1;
	tmp = *cmds;
	prev = NULL;
	while (++cmd_id < lst_size && tmp)
	{
		if (prev)
			if (prev->output_fd < 2 && tmp->input_fd < 2
				&& prev->path_to_cmd && prev->is_builtin == 0)
				tmp->input_fd = prev->pipe_fds[0];
		prev = tmp;
		tmp = tmp->next;
	}
	return (*cmds);
}

t_cmd	*link_output_pipes(t_cmd **cmds, t_mshell *data)
{
	t_cmd	*tmp;
	int		cmd_id;
	int		lst_size;

	if (!cmds || !*cmds || !data)
		return (NULL);
	lst_size = cmd_list_size(cmds);
	cmd_id = -1;
	tmp = *cmds;
	while (++cmd_id < lst_size && tmp)
	{
		printf(RESET "LOP\tID[%d] size-1[%d]\n", cmd_id, lst_size - 1);
		if (tmp->output_fd < 2 && cmd_id < lst_size - 1)
			tmp->output_fd = tmp->pipe_fds[1];
		else if (tmp->output_fd < 2 && cmd_id == lst_size - 1)
			tmp->output_fd = 0;
		tmp = tmp->next;
	}
	return (*cmds);
}

t_cmd	*manage_first_input(t_cmd *first_cmd)
{
	if (!first_cmd)
		return (NULL);
	if (first_cmd->input_fd < 2)
		first_cmd->input_fd = 0;
	return (first_cmd);
}

t_cmd	*convert_tokens_and_add_redirections(t_parse *parsing, char	**tokens,
	t_mshell *data)
{
	int		i;
	t_cmd	*cmd_list;

	if (!parsing || !tokens || !data)
		return (NULL);
	i = -1;
	cmd_list = NULL;
	while (tokens[++i])
	{
		printf(GREEN "CMD TOKEN[%d] |%s|\n", i, tokens[i]);
		if (!cmd_list)
			cmd_list = new_cmd_from_tokens(tokens[i], parsing, data, &i);
		else
			last_cmd(&cmd_list)->next = new_cmd_from_tokens(tokens[i],
				parsing, data, &i);
		if (!cmd_list)
			return (NULL);
	}
	cmd_list = link_input_pipes(&cmd_list, data);
	cmd_list = link_output_pipes(&cmd_list, data);
	cmd_list = manage_first_input(cmd_list);
	return (cmd_list);
}

static void	init_checks(t_cmd *cmd, t_f_check checks[4])
{
	if (!cmd || !checks)
		return ;
	if (cmd->input_fd > 2)
		checks[0] = f_access_check("fd", &cmd->input_fd);
	if (cmd->output_fd > 2)
		checks[1] = f_access_check("fd", &cmd->output_fd);
	checks[2] = f_access_check("fd", &cmd->pipe_fds[0]);
	checks[3] = f_access_check("fd", &cmd->pipe_fds[1]);
}

int	close_all_fds(t_cmd **cmds, t_mshell *data)
{
	t_cmd		*tmp;
	t_f_check	fds_check[4];

	if (!cmds || !*cmds || !data)
		return (1);
	tmp = *cmds;
	while (tmp)
	{
		init_checks(tmp, fds_check);
		if (fds_check[FD_IN].exists == TRUE)
			close(tmp->input_fd);
		if (fds_check[FD_OUT].exists == TRUE)
			close(tmp->output_fd);
		if (fds_check[PI_READ].exists == TRUE)
			close(tmp->pipe_fds[0]);
		if (fds_check[PI_WRITE].exists == TRUE)
			close(tmp->pipe_fds[1]);
		tmp = tmp->next;
	}
	return (0);
}

t_cmd	*build_cmds_list(t_parse *parsing, t_mshell *data)
{
	t_cmd	*cmd_list;
	char	**cmd_tokens;

	if (!parsing || !data)
		return (NULL);
	cmd_tokens = split_input_by_pipes(parsing->input, data);
	if (!cmd_tokens)
		return (NULL);
	printf("CMD TOKENS OK\n");
	cmd_list = convert_tokens_and_add_redirections(parsing, cmd_tokens, data);
	if (!cmd_list)
		return (NULL);
	printf("CMDLIST @%p\n", cmd_list);
	return (cmd_list);
}

//GESTION BUILTINS!
