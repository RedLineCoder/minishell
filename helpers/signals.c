/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:16:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/29 10:07:27 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_interrupt_main(int signal)
{
	(void)signal;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_interrupt_heredocs(int signal)
{
	(void)signal;
	ft_putstr_fd("\n", STDOUT_FILENO);
	exit(130);
}

void	handle_interrupt_child(int signal)
{
	(void)signal;
	exit(130);
}

void	handle_signals(t_job job)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	if (job == WAITING_INPUT)
		signal(SIGINT, handle_interrupt_main);
	else if (job == EXECUTING_HDOC)
		signal(SIGINT, handle_interrupt_heredocs);
	else if (job == EXECUTING_CMD)
		signal(SIGINT, handle_interrupt_child);
	else
		signal(SIGINT, SIG_IGN);
}
