/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:46:48 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/18 22:44:51 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/garbage_collector/garbage_collector.h"
# include "libft/libft.h"
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define SIG_NB 3
# define EXPORT_FORBIDDEN_CHARS "!@#$%%^&*-+={}[]()|\\/?><,.:;"
// # define

typedef struct s_envar	t_envar;

typedef enum e_envmod
{
	MOD_OK,
	MOD_KO,
	NO_VAR
}						t_envmod;

typedef enum e_export_return
{
	EXP_OK,
	EXP_ERROR,
	NODATA
}						t_expret;

typedef enum e_env_return
{
	ENV_EMPTY,
	ENV_FULL,
	ENV_ERROR
}						t_envret;

typedef enum e_sighandlers_id
{
	CTRL_C,
	CTRL_D,
	CTRL_BS
}						t_sighdlrid;

typedef struct s_envar
{
	char				*name;
	char				*value;
	t_envar				*next;
}						t_envar;

typedef struct s_mshell
{
	struct sigaction	*sighandlers;
	t_collector			*gc;
	t_envar				*env;
}						t_mshell;

void					clean_exit(t_mshell *data);

// INIT
t_bool					signal_handlers_setup(t_mshell *data);
int						build_var_list(char **env, t_mshell *data);
t_envar					*env_cpy(char *full_var, t_mshell *data);
t_envar					*get_last_var(t_envar *start);
int						update_var(t_mshell *data, char *name,
							char *new_value);

#endif