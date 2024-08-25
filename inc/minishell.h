/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:46:48 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/25 12:16:26 by sgoldenb         ###   ########.fr       */
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
# include <limits.h>

# define SIG_NB 3
# define EXPORT_FORBIDDEN_CHARS "!@#$%%^&*-+={}[]()|\\/?><,.:;\0"
# define MANAGED_QUOTES "\'\""
# define SQ_SEP '\x1F'
# define DQ_SEP '\x1E'
# define CMD_SEP '\x1D'
# define VAR_SEP 'A'
# define UNMANAGED_MCHARS "\\;"
# define ECHO_ESCAPE_SEQUENCES "\n\t\b\r\a\v\f\\" //\x to include

typedef struct s_envar	t_envar;

typedef enum e_envmod
{
	MOD_OK,
	MOD_KO,
	NO_VAR
}						t_envmod;

typedef enum e_varcheck
{
	VARS_FOUND,
	VARS_NONE,
	VARS_ERROR
}						t_varcheck;

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

typedef struct e_expcheck
{
	int					sq_count;
	int					dq_count;
	int					cmd_exp_count;
	int					var_count;
	t_envar				**vars_to_insert;
	char				*to_expand;
}						t_expand;

typedef struct s_envar
{
	char				*name;
	char				*value;
	t_envar				*next;
	t_bool				hidden;
}						t_envar;

typedef struct s_mshell
{
	struct sigaction	*sighandlers;
	int					pipe_fds[1024];
	t_collector			*gc;
	t_envar				*env;
	char				***cmds;
}						t_mshell;

void					clean_exit(t_mshell *data);

// INIT
t_bool					signal_handlers_setup(t_mshell *data);
int						build_var_list(char **env, t_mshell *data);
t_envar					*env_cpy(char *full_var, t_mshell *data);

//VAR MANAGEMENT
t_envar					*get_last_var(t_envar *start);
int						update_var(t_mshell *data, char *name,
							char *new_value);
int						print_env(t_mshell *data);
t_envar					*new_var(char *name, char *value, t_mshell *data,
							t_bool hide);

//STRING EXPANSION
void					place_separator(t_expand *str, char to_replace);
void					count_quotes(t_expand *str);
int						count_cmds(char *raw_input);
t_envar					*search_var(t_envar **first_var, char *var_name);
t_expand				*new_expansion(char *str, t_mshell *data);
char					*get_var_name(char *var_start, t_mshell *data);
char					**fill_var_names(char *to_expand, int var_count,
							t_mshell *data, char **names);
int						mark_vars(t_expand *str, t_mshell *data);
t_bool					validate_var(char *var_start);
void					place_var_sep(t_expand *exp, t_mshell *data);
t_bool					ft_isvarname(char var_char);

//BUILTINS
int						builtin_error(char *builtin_name, char *args,
							int errnum, t_mshell *data);
#endif