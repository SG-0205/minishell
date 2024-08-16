/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:43:51 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/16 10:13:16 by sgoldenb         ###   ########.fr       */
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

void	print_env(t_mshell *data)
{
	int	i;

	i = -1;
	while (data->env[++i])
		printf("ENV[%d] = %s\n", i, data->env[i]);
}

void	print_path(t_mshell *data)
{
	int	i;

	i = -1;
	while (data->path[++i])
		printf("PATH[%d] = %s\n", i, data->path[i]);
}

char	**split_path(t_mshell *data)
{
	int	i;
	int	size;

	i = -1;
	if (!data || !data->env)
		return (NULL);
	data->path = gc_split(gc_strtrim(ft_arrfind(data->env, "PATH="), "PATH=",
				data->gc, 0), ':', data->gc, 0);
	if (!data->path)
		return (NULL);
	return (NULL);
}

char	**env_dup(t_mshell *data, char **env)
{
	int		i;
	int		size;
	char	**new;

	if (!env || !*env || !data)
		return (NULL);
	i = -1;
	size = ft_arrlen((void **)env);
	new = (char **)gc_malloc(data->gc, sizeof(char *) * (size + 1), 0);
	if (!new)
		return (NULL);
	while (++i < size)
	{
		new[i] = (char *)gc_malloc(data->gc, sizeof(char *) * (ft_strlen(env[i])
				+ 1), 0);
		if (!new[i])
			return (NULL);
		new[i] = ft_strcpy(new[i], env[i]);
	}
	new[i] = NULL;
	return (new);
}

t_bool	init_data(t_mshell *data, char **env)
{
	if (!data)
		return (FALSE);
	data->gc = gc_init(1);
	if (!data->gc)
		return (FALSE);
	data->env = env_dup(data, env);
	if (!data->env)
		return (FALSE);
	print_env(data);
	data->env = split_path(data);
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
	printf("%s\n", env[1]);
	while (1)
	{
		buffer = readline(GREEN ITALIC "Minishell> " RESET);
		if (empty_line(buffer) == FALSE)
			add_history(buffer);
		if (!buffer)
		{
			printf("EOF\n");
			break ;
		}
		printf("%s\n", buffer);
	}
	clear_data(data);
	free(buffer);
	return (0);
}
