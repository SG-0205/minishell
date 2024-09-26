/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_printers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:26:40 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/09/26 16:35:00 by sgoldenb         ###   ########.fr       */
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

int	print_redirection_list(t_redirs *redirs)
{
	t_redirs	*tmp;
	int			i;
	
	if (redirs)
		printf(YELLOW "PRINT_REDIRECTIONS @%p\n---------\n" RESET, redirs);
	else
		return (printf(RED BOLD "NO_DATA\n" RESET));
	tmp = redirs;
	i = 0;
	while (tmp)
	{
		printf(GREEN "[%d]\t" RESET, i);
		printf(CYAN "FD = %d\t" RESET, tmp->fd);
		printf(RED "CMD_ID = %d\t" RESET, tmp->cmd_id);
		if (tmp->type == HEREDOC)
			printf(MAGENTA "HEREDOC\n" RESET);
		else if (tmp->type == INPUT)
			printf(MAGENTA "INPUT\n" RESET);
		else if (tmp->type == OUTPUT)
			printf(MAGENTA "OUTPUT\n" RESET);
		else if (tmp->type == APPEND)
			printf(MAGENTA "APPEND\n\n" RESET);
		tmp = tmp->next;
		i ++;
	}
	return (0);
}
