/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_vars_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 12:42:33 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/25 13:33:59 by sgoldenb         ###   ########.fr       */
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

static void	cpy_var_name(char *new_c, char *old_c, int *i, int *j)
{
	*new_c = *old_c;
	*i += 1;
	*j += 1;
}

static void	cpy_and_sep_var(char *vars_marked, char *to_expand, int var_count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (to_expand[i] && j < (int)ft_strlen(to_expand) + var_count)
	{
		if (to_expand[i] == '$'
			&& validate_var(&to_expand[i]) == TRUE)
		{
			vars_marked[j] = *VAR_SEP;
			j++;
			i++;
			while (ft_isvarname(to_expand[i]) == TRUE)
				cpy_var_name(&vars_marked[j], &to_expand[i], &i, &j);
			vars_marked[j] = *VAR_SEP;
			j++;
		}
		else
		{
			vars_marked[j] = to_expand[i];
			j++;
			i++;
		}
	}
}

void	place_var_sep(t_expand *exp, t_mshell *data)
{
	char	*vars_marked;

	if (!exp || !exp->to_expand || exp->var_count < 1)
		return ;
	vars_marked = (char *)gc_strnew((sizeof(char)
				* (ft_strlen(exp->to_expand) + exp->var_count)), data->gc, 0);
	if (!vars_marked)
		return ;
	cpy_and_sep_var(vars_marked, exp->to_expand, exp->var_count);
	exp->to_expand = gc_strdup(vars_marked, data->gc, 0);
	if (!exp->to_expand)
		return ;
}

//FAIRE UN PUTAIN DE SCHEMA POUR CETTE PUTAIN DE CONCATENATION - 24/08
//LA PUTAIN DE CONCATENATION EST OK!!! - 25/08

//TODO VERIFIER CONDITIONS DE DELIMITATION DES VARIABLES , SAVOIR QUE FAIRE DE &
//& SERA CONSIDERE COMME EOV - 24/08