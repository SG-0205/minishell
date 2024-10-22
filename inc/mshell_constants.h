/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell_constants.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:55:27 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/22 18:37:01 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSHELL_CONSTANTS_H
# define MSHELL_CONSTANTS_H

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
# define SEPS "\x1A\x1D\x1C\v\x06\x1D "
# define ECHO_ESCAPE_SEQUENCES "\n\t\b\r\a\v\f\\" //\x to include

#endif