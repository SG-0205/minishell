/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reinit_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:26:21 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/05 15:38:44 by sgoldenb         ###   ########.fr       */
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

char	*get_pwd(t_mshell *data)
{
	if (!data)
		return (NULL);
	if (!search_var(&data->env, "PWD"))
		reinit_pwd(data);
	return (dup_var_value(data, "PWD"));
}
