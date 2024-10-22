/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell_types.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:55:30 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/22 11:17:14 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSHELL_TYPES_H
# define MSHELL_TYPES_H

# include "libft/libft.h"
# include "mshell_constants.h"

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
	t_bool				bad_hd;
	t_pidlist			*pids;
}								t_mshell;

#endif