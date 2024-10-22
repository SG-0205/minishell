/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell_enums.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:55:31 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/22 10:56:35 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSHELL_ENUMS_H
# define MSHELL_ENUMS_H

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

#endif