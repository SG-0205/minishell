/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:50:05 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/09 06:10:24 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	special_print(t_mshell *data)
{
	char	**splitted_env;
	int		i;

	if (!data)
		return ;
	splitted_env = env_list_to_array(data, FALSE);
	if (!splitted_env || !*splitted_env)
		return ;
	i = -1;
	while (splitted_env[++i])
		printf("export %s\n", splitted_env[i]);
}

static t_bool	check_args(char *args)
{
	int	i;

	i = -1;
	if (ft_isdigit(args[0]) != 0 || ft_strcmp(args, "=\0") == 0)
		return (FALSE);
	while (args[++i] && args[i] != '=')
	{
		if (ft_cisar(args[i], EXPORT_FORBIDDEN_CHARS) == 1
			|| ft_cisar(args[i], "?") == 1)
			return (FALSE);
	}
	return (TRUE);
}

static void	update_env(char *arg, t_mshell *data)
{
	char	*name;
	char	*value;

	if (!arg || !data)
		return ;
	name = gc_strnew(ft_lentillc(arg, '='), data->gc, 1);
	if (!name)
		return ;
	name = ft_strncpy(name, arg, ft_lentillc(arg, '='));
	value = gc_strnew(ft_strlen(&arg[ft_lentillc(arg, '=') + 1]), data->gc, 1);
	if (!value)
		return ;
	value = ft_strncpy(value, &arg[ft_lentillc(arg, '=') + 1],
			ft_strlen(&arg[ft_lentillc(arg, '=') + 1]));
	if (ft_strcmp(value, "\0") == 0)
		return ;
	update_var(data, name, value);
}

int	export(t_mshell *data, char **args)
{
	int	i;

	if (!data)
		return (NODATA);
	if (!args || !*args)
	{
		special_print(data);
		return (0);
	}
	i = -1;
	while (args[++i])
		if (check_args(args[i]) == FALSE)
			return (custom_b_error("export", args[i],
					"not a valid identifier", data));
	i = -1;
	while (args[++i])
		update_env(args[i], data);
	return (0);
}
