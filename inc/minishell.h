/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:46:48 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/21 16:14:35 by sgoldenb         ###   ########.fr       */
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
# include <err.h>
# include <error.h>
# include <sys/stat.h>

# define SIG_NB 3
# define EXPORT_FORBIDDEN_CHARS "!@#$%%^&*-+={}[]()|\\/><,.:;\0"
# define PIPE_NBR 1024
# define PATH_CHARS "_./~,;:()[]{}+=$!@#&*?|\\^'\"`\n\r\t"
# define ARG_SEPARATORS " \t\r"
# define MANAGED_QUOTES "\'\""
# define HEREDOC_PATH "/mnt/nfs/homes/sgoldenb/sgoinfre/HD"
# define ARG_SEP "\x1A"
# define SQ_SEP "*"
# define DQ_SEP "\x1D"
# define CMD_SEP "\x1C"
# define VAR_SEP "\v"
# define RED_SEP "\x06"
# define R_S_SEP "\x1D"
# define BUILTINS_STR "cd.pwd.env.echo.exit.unset.export"
# define UNMANAGED_MCHARS "\\"
# define ECHO_ESCAPE_SEQUENCES "\n\t\b\r\a\v\f\\" //\x to include

extern sig_atomic_t				g_sig_trace;

typedef struct s_envar			t_envar;
typedef struct e_cmd			t_cmd;
typedef struct e_path_node		t_pn;
typedef struct e_path_stack		t_p_stack;
typedef struct s_redirections	t_redirs;
typedef struct s_file_check		t_f_check;
typedef struct s_pid_lists		t_pidlist;
typedef struct s_mshell			t_mshell;

typedef struct s_file_check
{
	t_bool						read;
	t_bool						write;
	t_bool						exec;
	t_bool						exists;
	t_bool						is_dir;
	t_bool						is_file;
	struct stat					path_stats;
	struct stat					fd_stats;
}								t_f_check;

typedef enum e_fd_end_check
{
	FD_IN,
	FD_OUT,
	PI_READ,
	PI_WRITE
}								t_fdchecks;

typedef enum e_heredoc_limit
{
	SQ,
	NORMAL,
	UNTAB,
	UNTAB_SQ,
}								t_hd_l_type;

typedef enum e_redirection_type
{
	OUTPUT,
	INPUT,
	APPEND,
	HEREDOC
}								t_redir_type;

typedef enum e_envsplit
{
	PUBLIC_VARS,
	HIDDEN_VARS,
	SPLIT_END
}								t_envsplit;

typedef enum e_envmod
{
	MOD_OK,
	MOD_KO,
	NO_VAR
}								t_envmod;

typedef enum e_varcheck
{
	VARS_FOUND,
	VARS_NONE,
	VARS_ERROR
}								t_varcheck;

typedef enum e_export_return
{
	EXP_OK,
	EXP_ERROR,
	NODATA
}								t_expret;

typedef enum e_env_return
{
	ENV_EMPTY,
	ENV_FULL,
	ENV_ERROR
}								t_envret;

typedef enum e_sighandlers_id
{
	CTRL_C,
	CTRL_BS
}								t_sighdlrid;

typedef struct s_pid_lists
{
	pid_t				pid;
	t_pidlist			*next;
}								t_pidlist;

typedef struct s_redirections
{
	int					fd;
	int					cmd_id;
	int					errcorde;
	char				*path;	//Ne pas renvoyer l'erreur en expand
	t_redir_type		type;
	t_redirs			*next;
}								t_redirs;

typedef struct s_parsing
{
	char				*input;
	char				**args;
	t_redirs			*redirections;
	t_cmd				*cmds;
	char				**tokens;
	int					ncmds;
}								t_parse;

typedef struct s_pid_lists
{
	pid_t				pid;
	t_pidlist			*next;
}						t_pidlist;

typedef struct s_redirections
{
	int					fd;
	int					cmd_id;
	t_redir_type		type;
	t_redirs			*next;
}						t_redirs;

typedef struct s_parsing
{
	char				*input;
	char				**args;
	t_redirs			*redirections;
	t_cmd				*cmds;
	
}						t_parse;

typedef struct e_expcheck
{
	int					sq_count;
	int					dq_count;
	int					cmd_exp_count;
	int					var_count;
	t_envar				**vars_to_insert;
	char				*to_expand;
	char				*expanded;
}								t_expand;

typedef struct e_path_node
{
	char				*dir;
	t_pn				*next;
}								t_pn;

typedef struct e_path_node
{
	char				*dir;
	t_pn				*next;
}						t_pn;

typedef struct e_cmd
{
	char				*cmd_name;
	int					exit;
	char				*path_to_cmd;
	char				**args;
	int					pipe_fds[2];
	pid_t				pid;
	int					input_fd;
	int					output_fd;
	char				**env;
	int					is_builtin;
	int					skip;
	t_redirs			*redirs;
	t_mshell			*link;
	t_cmd				*next;
	t_cmd				*prev;
}								t_cmd;

typedef struct s_envar
{
	char				*name;
	char				*value;
	t_envar				*next;
	t_bool				hidden;
}								t_envar;

typedef struct s_mshell
{
	struct sigaction	*sighandlers;
	int					fds[PIPE_NBR][2];
	t_collector			*gc;
	t_envar				*env;
	t_bool				no_exec;
	t_pidlist			*pids;
}								t_mshell;

typedef struct s_initialestruct
{
	int			argc;
	char		**argv;
	char		**env;
	t_parse		p;
}								t_initialestruct;

void					clean_exit(t_mshell *data);
int						print_redirection_list(t_redirs *redirs,
							t_mshell *data);
int						print_cmd_list(t_cmd *start);
int						print_pid_list(t_pidlist *start);

// INIT
t_bool					signal_handlers_setup(t_mshell *data);
int						build_var_list(char **env, t_mshell *data);
t_envar					*env_cpy(char *full_var, t_mshell *data);
void					clear_data(t_mshell *data);

//VAR MANAGEMENT
t_envar					*get_last_var(t_envar *start);
int						update_var(t_mshell *data, char *name,
							char *new_value);
int						print_env(t_mshell *data, t_envsplit to_print);
t_envar					*new_var(char *name, char *value, t_mshell *data,
							t_bool hide);
t_envar					*dup_var(t_envar *var, t_mshell *data);
char					**env_list_to_array(t_mshell *data,
							t_bool hidden);
int						env_size(t_envar **env_start, t_bool count_hidden);
int						var_list_position(t_envar **first_var, t_envar *var);
void					reinit_pwd(t_mshell *data);
t_envar					**get_side_vars(t_envar *var, t_mshell *data);
char					*dup_var_value(t_mshell *data, char *var_name);
t_envar					**split_env_on_visibility(t_envar **env_start,
							t_mshell *data);
char					**env_list_to_array(t_mshell *data, t_bool hidden);
char					*join_var_name_and_value(t_envar *var, t_mshell *data);
int						env_size(t_envar **env_start, t_bool count_hidden);

//PATH MANAGEMENT
char					*get_pwd(t_mshell *data);
char					*extend_path(char *path, t_mshell *data);
t_pn					*last_dir(t_pn *start);
t_pn					*new_path_node(char *dir, t_mshell *data);
void					rm_last_dir(t_pn *start);
void					add_dir(char *dir, t_pn *list, t_mshell *data);
int						full_path_len(t_pn *start);
t_bool					is_path(char *path_start);
t_bool					is_path_char(char c);
t_bool					is_builtin(char **tokens, t_cmd *cmd, t_mshell *data);
char					**dup_cmd_args(char	**tokens, t_mshell *data);

//REDIRECTIONS
char					*extract_content(int fd, t_mshell *data);
t_f_check				f_access_check(char *f_path, int *fd);
t_f_check				errored_object(void);
t_bool					is_a_redirection(char *arg);
t_redir_type			read_redirection_type(char *arg);
char					*mark_redirections(char *input, t_mshell *data);
int						locate_redirection(char **args,
							t_redir_type redir_type);
char					*heredoc(char *limiter,
							t_hd_l_type type, t_mshell *data);
int						heredoc_fd(char *raw_limiter, t_mshell *data);
char					**split_redirections(char *input, t_mshell *data);
t_redirs				*extract_redirections(t_parse *parse, t_mshell *data);
t_bool					has_redir(char *input);
t_redirs				*new_redirection(int *fd, int *cmd_id,
							t_redir_type *type, t_mshell *data);
t_redirs				*get_last_redir(t_redirs **start);
t_redirs				*get_last_redir_by_cmd_id(t_redirs **start, int cmd_id);
t_redirs				*filter_redirs_by_type(t_redirs **origin,
							t_redir_type type, t_mshell *data);
t_redirs				*dup_redirection(t_redirs **elem_addr, t_mshell *data);
t_bool					init_data(t_mshell *data, char **env);
t_bool					has_redirs_by_id(int *cmd_id, t_redirs *redirs);


//COMMANDS
t_cmd					*last_cmd(t_cmd **start);
t_cmd					*new_empty_cmd(t_mshell *data);
int						cmd_list_size(t_cmd **start);
int						close_all_fds(t_cmd *cmds);
t_cmd					*build_cmds_list(t_parse *parsing, t_mshell *data);
void					exec_cmds(t_cmd *cmds);
void					close_non_pipes_fds(t_cmd *start);
t_cmd					*manage_first_input(t_cmd *first_cmd);
t_cmd					*link_output_pipes(t_cmd **cmds, t_mshell *data);
t_cmd					*link_input_pipes(t_cmd **cmds, t_mshell *data);
t_cmd					*new_cmd_from_tokens(char *tokens, t_parse *parsing,
							t_mshell *data);
char					*interpolate_path(char *cmd_name,
							t_mshell *data);
int						is_not_operator(char c);
void					update_return_code(int exit_code, t_mshell *data);
char					*rm_tabulations(char *final);
t_bool					check_red_syntax(char **red_tokens,
							char *ref, t_mshell *data);

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
t_bool					is_sep(char c);
t_bool					is_relative_path(char *path, t_mshell *data);
t_bool					is_quoted(char *ptr_in_str, char *str,
							t_bool check_both);
t_bool					is_quoted_by(char quote_type, char *ptr_in_str,
							char *str);
void					separator_mitigation(t_expand *exp);
void					str_shrink(char *str);
char					*str_insert(char *str, char *where_to_insert,
							char *to_insert, t_mshell *data);
int						ft_lentillptr(char *c, char *str);
char					first_quoting(char *ptr_in_str, char *str);
char					ret_quoting(char *ptr_in_str, char *str);
char					**initial_split(char *input, t_mshell *data);
t_bool					is_closed(char *quote_ptr, char *str);
int						read_dq(char *dq_start, char *arg, char *new_arg,
							t_mshell *data);
int						read_sq(char *sq_start, char *arg, char *new_arg);
t_bool					is_a_closing_quote(char *quote_ptr, char *str);
t_bool					is_closed(char *quote_ptr, char *str);
char 					**expand_all_args(char **args, t_mshell *data);
char					*quote_closure_control(char *input);
char					*read_quoting(char *arg, t_mshell *data);
int						insert_var(char *var_start, char *arg, t_mshell *data,
							char *n_arg);
int						try_insert_value(char *var_start, char *container,
							t_mshell *data);
int						ft_lentillptr(char *c, char *str);
char					*args_separation(char *input);
char					*revert_unclosed(char *input);
char					*rm_redirections(char *input);
t_bool					check_bad_limiter(char *limiter, t_mshell *data);
void					*bad_eof_hd(char *limiter, int l_count,
							t_mshell *data);
t_hd_l_type				get_hd_output_type(char *limiter);
t_bool					empty_line(char *input);
t_cmd					*apply_redirections(t_cmd *cmd, t_parse *parsing,
							int *cmd_id, t_mshell *data);
char					*convert_rp(char *relative_path, t_mshell *data);
char					*path_search(char *to_search, t_mshell *data);
t_bool					check_2_quotes(char *ptr_in_str, char *str);
int						cpy_redir(char *red_start, char *new);
void					*special_filter(char *fault, t_mshell *data);
int						redirect(t_cmd *cmd);
t_bool					check_empty_cmd(char **tokens,
							t_parse *parsing, t_mshell *data);
void					save_exit_code(t_cmd *tmp, int *process_exit);
t_bool					local_exec(t_cmd *cmd, t_cmd *start);
t_bool					is_single_cmd(char **input);
int						r_id_by_reference(char **r_token, char **ref);
int						try_open(char *path,
							t_redir_type type, t_mshell *data);
void					bruteforce_pipes(t_mshell *data, int keep[2]);

//SIGNALS
int						save_pid(pid_t process_pid, t_mshell *data);
int						kill_processes(int signal, t_mshell *data);


//BUILTINS
int						builtin_error(char *builtin_name, char *args,
							int errnum, t_mshell *data);
int						custom_b_error(char *builtin_name, char *args,
							char *custom_msg, t_mshell *data);
int						syntax_error(char *faulty_token, t_mshell *data);
int						bad_eof(char *limiter, int l_count, t_mshell *data);
char					*extend_relative_path(char *path, t_mshell *data);
int						export(t_mshell *data, char **args);
int						cd(char **path, t_mshell *data);
int						unset(char **args, t_mshell *data);
int						print_exp(t_expand *exp, char *location);
int						exit_b(char **args, t_mshell *data);
int						pwd(char **args, t_mshell *data);
int						echo_b(char **args, t_mshell *data);
int						error_full_len(char **args);
int						custom_shell_error(char *arg, char *custom_msg,
							int m_errcode, t_mshell *data);
int						mshell_error(char *faulty_arg, int errnum,
							t_mshell *data);
char					*quote_e_args(char *args, t_bool enforce,
							t_mshell *data);
int						normalize_return(int exit_code);
int						exec_builtins(char *b_name,
							char **b_args, t_mshell *data);
char					**split_input_by_pipes(char *input, t_mshell *data);

//EXEC
int						ft_exec(t_cmd *list);
int						ft_loop(t_cmd *list);
int						ft_execute(t_cmd *list);
int						ft_parent(t_cmd *list, int *i);
int						ft_child(t_cmd *list, int *i);
t_bool					is_local(t_cmd *cmd, t_cmd *start);
t_bool					try_redirections(t_cmd *cmd, int *i);
t_cmd					*get_first_item(t_cmd *end);
t_initialestruct		*s(void);

#endif
//cat

//cat <infile | cat
//err + cat

//cat noinfile | ls
//err + ls

//cat noinfile | echo ok
//err + echo ok

//cat noinfile | grep a
//err + grep prend r

//cat <infile | wc
//err + wc

//si c un builtin : execution du builtin apres








//echo
