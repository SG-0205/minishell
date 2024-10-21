/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_printers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:26:40 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/21 16:14:25 by sgoldenb         ###   ########.fr       */
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

int	print_redirection_list(t_redirs *redirs, t_mshell *data)
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
		printf(GREEN "[%d][%p]\t" RESET, i, tmp);
		printf(CYAN "FD = %d\t" RESET, tmp->fd);
		printf(RED "CMD_ID = %d\t" RESET, tmp->cmd_id);
		printf(CYAN "ERRNOSAVE = %d\t" RESET, tmp->errcorde);
		printf(BLUE "F_PATH = %s\t" RESET, tmp->path);
		if (tmp->type == HEREDOC)
			printf(MAGENTA "HEREDOC\n" RESET);
		else if (tmp->type == INPUT)
			printf(MAGENTA "INPUT\n" RESET);
		else if (tmp->type == OUTPUT)
			printf(MAGENTA "OUTPUT\n" RESET);
		else if (tmp->type == APPEND)
			printf(MAGENTA "APPEND\n" RESET);
		printf("DATA %p\n", data);
		tmp = tmp->next;
		i ++;
	}
	return (0);
}

int	print_cmd_list(t_cmd *start)
{
	t_cmd	*tmp;
	int		count;
	int		total;

	if (!start)
	{
		printf(BOLD RED "NO CMDS\n" RESET);
		return (-1);
	}
	total = cmd_list_size(&start);
	tmp = start;
	count = 0;
	printf(YELLOW "PRINT_CMDS @%p (%d CMDS)\n---------\n" RESET, start, total);
	while (tmp && ++count <= total)
	{
		printf(GREEN "[%d/%d]\n" RESET, count, total);

		//s()->p.ncmds = total;
		//tmp->exit = count;
		//if (tmp->path_to_cmd && *tmp->path_to_cmd)
			//printf(BOLD "\tPATH: %s\n" RESET, tmp->path_to_cmd);
		printf(RED "PREV @%p\t NEXT@%p\n" RESET, tmp->prev, tmp->next);
		if (tmp->env)
			printf(GREEN "\t\t[HAS ENV]\n" RESET);
		else
			printf("\t\t[NO ENV]\n");
		if (tmp->is_builtin == 0)
			printf(BLUE "\t\t[IS EXTERN]\n" RESET);
		else
			printf(YELLOW "\t\t[IS BUILTIN]\n" RESET);
		printf(BOLD "\tARGS:\n" RESET);
		for (int i = 0; tmp->args && tmp->args[i]; i++)
			printf(MAGENTA "\t\t[%d]:%s\n" RESET, i, tmp->args[i]);
		printf(CYAN "\tINPUT_FD: %d\n" RESET, tmp->input_fd);
		printf(RED "\tOUTPUT_FD: %d\n" RESET, tmp->output_fd);
		printf(BOLD "\tPIPES_FD[IN %d / OUT %d]\n" RESET, tmp->pipe_fds[1], tmp->pipe_fds[0]);
		printf("----------\n\n");
		tmp = tmp->next;
	}
	return (0);
}

int	print_pid_list(t_pidlist *start)
{
	t_pidlist	*tmp;
	int			count;

	if (!start)
	{
		printf(RED "NO PIDS\n" RESET);
		return (-1);
	}
	count = -1;
	tmp = start;
	printf(YELLOW "PRINT_PIDS @%p\n----------\n" RESET, start);
	while (tmp)
	{
		count ++;
		printf(GREEN "[%d]\n" RESET, count);
		printf(CYAN "\tPID >%d<\n" RESET, tmp->pid);
		printf("------\n\n");
		tmp = tmp->next;
	}
	return (0);
}
