/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:19:44 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/10/03 14:54:19 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	new_prompt(int signal)
{
	if (signal == SIGINT)
	{
		// if (sig_code != signal)
		// 	sig_code = signal;
		rl_on_new_line();
		printf("\n");
		rl_replace_line("\0", 1);
		rl_redisplay();
	}
}

t_bool	new_sig_handler(struct sigaction new, t_sighdlrid sig_type)
{
	new.sa_flags = 0;
	sigemptyset(&new.sa_mask);
	if (sig_type == CTRL_C)
	{
		new.sa_handler = new_prompt;
		if (sigaction(SIGINT, &new, NULL))
			return (FALSE);
		return (TRUE);
	}
	if (sig_type == CTRL_D)
	{
		new.sa_handler = new_prompt;
		if (sigaction(555, &new, NULL) == -1)
			return (FALSE);
	}
	if (sig_type == CTRL_BS)
	{
		new.sa_handler = new_prompt;
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
	return (new_sig_handler(data->sighandlers[CTRL_C], CTRL_C));
}
