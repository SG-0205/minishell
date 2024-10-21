/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:19:44 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/08 12:56:11 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	new_prompt_and_kill(int signal)
{
	if (signal == SIGINT && g_sig_trace != SIGINT)
	{
		wait(NULL);
		rl_on_new_line();
		rl_replace_line("", 0);
		printf("\n");
		rl_redisplay();
		return ;
	}
}

void	nothing_lol(int signum)
{
	printf("SIGNB %d\n", signum);
	if (signum == SIGQUIT)
	{
		signal(SIGQUIT, SIG_IGN);
	}
}

t_bool	new_sig_handler(struct sigaction new, t_sighdlrid sig_type)
{
	new.sa_flags = 0;
	sigemptyset(&new.sa_mask);
	if (sig_type == CTRL_C)
	{
		new.sa_handler = new_prompt_and_kill;
		if (sigaction(SIGINT, &new, NULL) == -1)
			return (FALSE);
	}
	return (TRUE);
}

t_bool	signal_handlers_setup(t_mshell *data)
{
	if (!data)
		return (FALSE);
	data->sighandlers = (struct sigaction *)gc_malloc(data->gc,
			sizeof(struct sigaction) * SIG_NB, 0);
	if (!data->sighandlers)
		return (FALSE);
	signal(SIGQUIT, SIG_IGN);
	return (new_sig_handler(data->sighandlers[CTRL_C], CTRL_C));
}
