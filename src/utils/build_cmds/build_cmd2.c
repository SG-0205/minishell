/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:43:44 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/09 03:17:01 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

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

void	*special_filter(char *fault, t_mshell *data)
{
	if (!fault || !data)
		return (NULL);
	if (ft_strcmp(fault, ".") == 0)
		custom_shell_error(fault, "filename argument required", 2, data);
	else if (ft_strcmp(fault, "..") == 0)
		custom_shell_error(fault, "command not found\0", 127, data);
	data->no_exec = TRUE;
	return (NULL);
}

char	*path_search(char *to_search, t_mshell *data)
{
	int		i;
	char	**path_dirs;
	t_envar	*path_search_try;

	if (!to_search || !data)
		return (NULL);
	if (ft_strcmp(to_search, "..") == 0 || ft_strcmp(to_search, ".") == 0)
		return (special_filter(to_search, data));
	if (access(to_search, X_OK | F_OK) == 0)
		return (to_search);
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
	data->no_exec = TRUE;
	return (NULL);
}

char	*convert_rp(char *relative_path, t_mshell *data)
{
	char		*converted;
	t_f_check	checks;
	int			flag;

	if (!relative_path || !data)
		return (NULL);
	converted = extend_path(relative_path, data);
	if (!converted)
		return (NULL);
	flag = 0;
	checks = f_access_check(relative_path, NULL);
	if (checks.exists == FALSE)
		flag = custom_shell_error(relative_path, strerror(EACCES), 127, data);
	else if (checks.is_dir == TRUE)
		flag = custom_shell_error(relative_path, strerror(EISDIR), 126, data);
	else if (checks.exec == FALSE)
		flag = custom_shell_error(relative_path, strerror(EACCES), 126, data);
	if (flag == 0)
		return (converted);
	data->no_exec = TRUE;
	return (NULL);
}
