/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 21:50:46 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/29 00:26:43 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <fcntl.h>

# define RESET "\e[0m"
# define BOLD "\e[1m"
# define FAINT "\e[2m"
# define ITALIC "\e[3m"
# define UNDERLINE "\e[4m"
# define BLINK "\e[5m"
# define INVERSE "\e[7m"
# define HIDDEN "\e[8m"
# define STRIKETHROUGH "\e[9m"
# define RESET_BOLD "\e[21m"
# define RESET_FAINT "\e[22m"
# define RESET_ITALIC "\e[23m"
# define RESET_UNDERLINE "\e[24m"
# define RESET_BLINK "\e[25m"
# define RESET_INVERSE "\e[27m"
# define RESET_HIDDEN "\e[28m"
# define RESET_STRIKETHROUGH "\e[29m"

# define BLACK "\e[30m"
# define RED "\e[31m"
# define GREEN "\e[32m"
# define YELLOW "\e[33m"
# define BLUE "\e[34m"
# define MAGENTA "\e[35m"
# define CYAN "\e[36m"
# define WHITE "\e[37m"
# define LIGHT_BLACK "\e[90m"
# define LIGHT_RED "\e[91m"
# define LIGHT_GREEN "\e[92m"
# define LIGHT_YELLOW "\e[93m"
# define LIGHT_BLUE "\e[94m"
# define LIGHT_MAGENTA "\e[95m"
# define LIGHT_CYAN "\e[96m"
# define LIGHT_WHITE "\e[97m"

typedef struct e_cmd	t_cmd;

typedef struct e_cmd
{
	char				*path_to_cmd;
	int					pipe_fds[2];
	int					input_fd;
	int					output_fd;
	char				**args;
	char				**env;
	int					append_out;
	int					is_redirected;
	t_cmd				*next;
}						t_cmd;

size_t	ft_envlen(char **env)
{
	size_t	len;

	if (!env || !*env)
		return (0);
	len = 0;
	while (env[len])
		len ++;
	return (len);
}

char	**ft_envdup(char **env)
{
	int		i;
	char	**dst;

	if (!env)
		return (NULL);
	dst = (char **)malloc(sizeof(char *) * (ft_envlen(env) + 1));
	if (!dst)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		dst[i] = ft_strdup(env[i]);
		if (!dst[i])
			return (NULL);
	}
	dst[i] = NULL;
	return (dst);
}

char	*ask_tester(char *title)
{
	char	*buffer;

	if (!title)
		return (NULL);
	buffer = readline(title);
	return (ft_strdup(buffer));
}

char	*find_cmd_with_path(char *cmd_name)
{
	int		i;
	char	**splitted_path;

	if (!cmd_name)
		return (NULL);
	i = -1;
	splitted_path = ft_split(getenv("PATH"), ':');
	if (!splitted_path)
		return (NULL);
	while (splitted_path[++i])
	{
		printf("[%d]=%s\n", i, splitted_path[i]);
		if (access(ft_strjoin(ft_strjoin(splitted_path[i], "/"), cmd_name), X_OK) == 0)
			return (ft_strjoin(ft_strjoin(splitted_path[i], "/"), cmd_name));
	}
	printf(RED "%s not found\n" RESET, cmd_name);
	return (NULL);	
}

int	fd_from_str(char *file_path, char *mode)
{
	int	o_modes;
	int	i;
	int	fd;

	o_modes = 0;
	if (!file_path)
	{
		printf(YELLOW "[no file provided]\n" RESET);
		return (-1);
	}
	else if (!mode)
	{
		printf(YELLOW "[no mode provided]\n" RESET);
		return (-1);
	}
	i = -1;
	while (mode[++i])
	{
		if (mode[i] == 'r')
		{
			if (o_modes == 0)
				o_modes = O_RDONLY;
			else
				o_modes |= O_RDONLY;
		}
		else if (mode[i] == 'w')
		{
			if (o_modes == 0)
				o_modes = O_WRONLY;
			else
				o_modes |= O_RDWR;
			o_modes |= O_CREAT | O_TRUNC;
		}
		else if (mode[i] == 'x')
			o_modes |= O_EXCL;				
	}
	fd = open(file_path, o_modes, S_IRUSR | S_IWUSR | S_IXUSR);
	if (fd < 1)
	{
		printf(RED "[error while opening file:%s(modes: %s) :%s]\n" RESET,
			file_path, mode, strerror(errno));
		return (-1);
	}
	return (fd);
}

char	**make_args(char *args_str)
{
	char	**args;

	if (!args_str)
	{
		printf(YELLOW "[no args inserted]\n" RESET);
		return (NULL);
	}
	args = ft_split(args_str, ' ');
	if (!args || !*args)
		return (NULL);
	return (args);
}

t_cmd	*last_cmd(t_cmd *list)
{
	t_cmd	*tmp;

	if (!list)
		return (NULL);
	tmp = list;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_cmd	*new_empty_cmd(void)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->append_out = -1;
	new->is_redirected = -1;
	new->next = NULL;
	new->output_fd = -1;
	new->input_fd = -1;
	new->path_to_cmd = NULL;
	new->args = NULL;
	new->env = NULL;
	if (pipe(new->pipe_fds) < 0)
	{
		printf(RED "[error while creating a cmd pipe]\n" RESET);
		return (NULL);
	}
	return (new);
}

t_cmd	*build_loop(char **env)
{
	t_cmd	*to_build;

	to_build = new_empty_cmd();
	if (!to_build)
		return (NULL);
	to_build->path_to_cmd = find_cmd_with_path(ask_tester("Cmd name? [ls/grep/etc...]> "));
	to_build->args = make_args(ask_tester("Cmd args?> "));
	if (ft_strcmp(ask_tester("Give env to cmd? [y/n]> "), "y") == 0)
		to_build->env = ft_envdup(env);
	if (ft_strcmp(ask_tester("Redirect input/output? [y/n]> "), "y") == 0)
	{
		to_build->input_fd = fd_from_str(ask_tester("Input [path/Ctrl+D to skip]> "),
			ask_tester("Opening modes [rwx/rx/x/wx/etc.../Ctrl+D to skip]> "));
		to_build->output_fd = fd_from_str(ask_tester("Output [path/Ctrl+D to skip]> "),
			ask_tester("Opening modes [rwx/rx/x/wx/etc.../Ctrl+D to skip]> "));
		if (to_build->input_fd != -1 || to_build->output_fd != -1)
			to_build->is_redirected = 1;
	}
	return (to_build);
}

t_cmd	*test_list(char **env)
{
	int		nb_cmds;
	int		i;
	t_cmd	*cmd_list;

	cmd_list = NULL;
	nb_cmds = ft_atoi(ask_tester("NB CMDS [int +]> "));
	i = 0;
	cmd_list = build_loop(env);
	while (++i < nb_cmds)
	{
		printf(MAGENTA "CMD[%d]\n", i);
		last_cmd(cmd_list)->next = build_loop(env);
	}
	return (cmd_list);
}

void	print_tab(char **tab)
{
	int	i;

	i = -1;
	if (!tab)
		return ;
	while (tab[++i])
		printf("\t\t[%d]:%s\n", i, tab[i]);
}

void	print_cmds(t_cmd *list)
{
	t_cmd	*tmp;
	int		count;

	if (!list)
	{
		printf(RED "NO COMMANDS\n" RESET);
		return ;
	}
	tmp = list;
	count = 0;
	printf(GREEN "------PRINT_CMDS------\n" RESET);
	while (tmp)
	{
		printf(CYAN "[%d]\n" RESET, count);
		printf(BOLD "\t> %s\n", tmp->path_to_cmd);
		printf("\tARGS:\n");
		print_tab(tmp->args);
		printf("\tFDS: IN[%d] OUT[%d]\n", tmp->input_fd, tmp->output_fd);
		if (tmp->pipe_fds)
			printf("PIPED\n\n" RESET);
		else
			printf("NO PIPE\n\n" RESET);
		count ++;
		tmp = tmp->next;
	}
	printf(GREEN "------END------\n");
}

int	main(int argc, char **argv, char **env)
{
	t_cmd	*list;

	list = test_list(env);            
	print_cmds(list);
}