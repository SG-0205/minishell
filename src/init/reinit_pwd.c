/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reinit_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:26:21 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/26 15:31:41 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	reinit_pwd(t_mshell *data)
{
	char	pwd[4096];

	if (!data || !data->env)
		return ;
	if (!getcwd(pwd, sizeof(pwd)))
		return ;
	update_var(data, "PWD", pwd);
}
