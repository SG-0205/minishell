/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_printers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:26:40 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/08/27 15:34:15 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_envar(t_envar *var)
{
	if (!var)
		return ;
	printf("\t@%p\n", var);
	printf("\t\t[HIDDEN : %d] %s=%s\n", var->hidden, var->name, var->value);
	if (var->next)
		printf("\t\tNEXT @%p\n", var->next);
}

int	print_exp(t_expand *exp, char *location)
{
	if (location)
		printf(CYAN "\nPRINT_EXP @ %s\n" RESET, location);
	else
		printf(CYAN "PRINT_EXP\n" RESET);
	if (!exp)
		return (printf(RED "NO_DATA\n" RESET));
	printf("BASE STR:\t%s\n", exp->to_expand);
	printf(GREEN "\tCOUNTS:\n" RESET);
	printf("\t\tSQ:\t%d\n\t\tDQ:\t%d\n", exp->sq_count, exp->dq_count);
	printf("\t\tVARS:\t%d\n\t\tCMDS:\t%d\n", exp->var_count, exp->cmd_exp_count);
	if (!exp->vars_to_insert || !*exp->vars_to_insert)
		printf(RED "\tNO_VARS\n" RESET);
	else
	{
		printf(YELLOW "\tVARS_TO_INSERT:\n" RESET);
		for (int i = 0; exp->vars_to_insert[i]; i++)
			print_envar(exp->vars_to_insert[i]);
	}
	printf("EXPANDED:\t%s\n\n", exp->expanded);
	return (0);
}
