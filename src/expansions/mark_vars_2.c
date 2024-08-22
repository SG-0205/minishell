/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_vars_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:42:33 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/22 20:38:54 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**fill_var_names(char *to_expand, int var_count,
	t_mshell *data, char **names)
{
	int	i;
	int	j;

	if (!to_expand || !data || !names)
		return (NULL);
	i = -1;
	j = 0;
	while (to_expand[++i] && var_count > 0)
	{
		if (to_expand[i] == '$'
			&& validate_var(&to_expand[i]) == TRUE)
		{
			names[j] = get_var_name(&to_expand[i], data);
			if (!names[j])
				return (NULL);
			j ++;
			var_count --;
		}
	}
	return (names);
}

void	place_var_sep(t_expand *exp, t_mshell *data)
{
	int		i;
	char	*vars_marked;

	if (!exp || !exp->to_expand || exp->var_count < 1)
		return ;
	i = -1;
	vars_marked = (char *)gc_strnew((sizeof(char)
				* (ft_strlen(exp->to_expand) + exp->var_count)), data->gc, 0);
	if (!vars_marked)
		return ;
	while (exp->to_expand[++i])
	{
		if (exp->to_expand[i] == '$'
			&& validate_var(&exp->to_expand[i]) == TRUE)
			vars_marked[i] = VAR_SEP;
		else if (ft_cisar(exp->to_expand[i], EXPORT_FORBIDDEN_CHARS) != 0
			|| ft_cisar(exp->to_expand[i], ECHO_ESCAPE_SEQUENCES)
			|| exp->to_expand[i] == '$' || exp->to_expand[i] == ' ')
			vars_marked[i] = VAR_SEP;
		else
			vars_marked[i] = exp->to_expand[i];
	}
	exp->to_expand = gc_strdup(vars_marked, data->gc, 0);
	if (!exp->to_expand)
		return ;
}

//TODO VERIFIER CONDITIONS DE DELIMITATION DES VARIABLES , SAVOIR QUE FAIRE DE &