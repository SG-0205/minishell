/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cmd3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:46:13 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/22 19:17:09 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static int	apply_redirection(t_redirs *redirections, t_redir_type i_o,
	int *cmd_id, t_mshell *data)
{
	t_redirs	*filtered_list;
	t_redirs	*final_item;
	int			def;

	if (!redirections || !data || (i_o != INPUT && i_o != OUTPUT))
		return (-1);
	if (i_o == INPUT)
		def = 0;
	else
		def = 1;
	filtered_list = filter_redirs_by_type(&redirections, i_o, data);
	if (!filtered_list)
		return (def);
	final_item = get_last_redir_by_cmd_id(&filtered_list, *cmd_id);
	if (!final_item)
		return (def);
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
	if (cmd->input_fd < 0)
		cmd->input_fd = 0;
	if (cmd->output_fd < 1)
		cmd->output_fd = 1;
	return (cmd);
}

char	*search_exception(char *cmd_name, char *new, t_mshell *data)
{
	if (!cmd_name || !new)
		return (NULL);
	if (ft_strcmp(cmd_name, new) == 0 && ft_strcmp(get_pwd(data), new) != 0)
		return (gc_strdup("command not found\0", data->gc, 1));
	else
		return (strerror(EISDIR));
}

char	*interpolate_path(char *cmd_name, t_mshell *data)
{
	char		*new;
	t_f_check	checks;
	int			flag;

	if (!cmd_name || !data || !*cmd_name)
		return (NULL);
	if (ft_strcmp(cmd_name, "..") == 0 || ft_strcmp(cmd_name, ".") == 0)
		return (special_filter(cmd_name, data));
	else if (is_relative_path(cmd_name, data) == TRUE)
		return (convert_rp(cmd_name, data));
	flag = 0;
	new = path_search(cmd_name, data);
	if (!new)
		flag = custom_shell_error(cmd_name, "command not found\0", 127, data);
	else
	{
		checks = f_access_check(new, NULL);
		if (checks.is_dir == TRUE)
			flag = custom_shell_error(cmd_name,
				search_exception(cmd_name, new, data), 127, data);
		else if (checks.exec == FALSE)
			flag = custom_shell_error(cmd_name, strerror(EACCES), 126, data);
	}
	if (flag != 0)
		return (NULL);
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
