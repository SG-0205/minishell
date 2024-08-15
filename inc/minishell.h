/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:46:48 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/15 22:56:02 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# include "libft/garbage_collector/garbage_collector.h"

# define SIG_NB 3

typedef enum e_sighandlers_id
{
	CTRL_C,
	CTRL_D,
	CTRL_BS
}						t_sighdlrid;

typedef struct s_mshell
{
	char				**env;
	struct sigaction	*sighandlers;
	t_collector			*gc;
}						t_mshell;

void	clean_exit(t_mshell *data);
t_bool	signal_handlers_setup(t_mshell *data);

#endif