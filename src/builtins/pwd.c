/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:06:31 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/14 21:19:20 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pwd(char **args, t_mshell *data)
{
	char		cwd[4096];
	t_f_check	pwd_checks;

	if (!data)
		return (-1);
	if (!args)
		return (builtin_error("pwd", NULL, ENODATA, data));
	if (!getcwd(cwd, sizeof(cwd)))
		return (builtin_error("pwd", NULL, errno, data));
	pwd_checks = f_access_check(cwd);
	if (pwd_checks.exists == FALSE)
		return (builtin_error("pwd", NULL, ENOENT, data));
	if (pwd_checks.exec == FALSE)
		return (builtin_error("pwd", NULL, EACCES, data));
	printf("%s\n", cwd);
	return (0);
}
