/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:16:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/11/03 15:13:27 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_interrupt_main(int signal)
{
	(void)signal;
	handle_sigint_output();
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_interrupt_heredocs(int signal)
{
	(void)signal;
	handle_sigint_output();
	exit(130);
}

void	handle_interrupt_child(int signal)
{
	(void)signal;
	exit(130);
}

int	set_termflags(t_msh *msh, t_term_action action)
{
	t_termios	termios;

	if (action == ADD_ECHOCTL)
	{
		if (tcgetattr(STDIN_FILENO, &termios))
			return (false);
		msh->termios = termios;
		termios.c_lflag |= ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios))
			return (false);
	}
	else if (action == SET_BACK
		&& tcsetattr(STDIN_FILENO, TCSAFLUSH, &msh->termios))
		return (false);
	return (true);
}

void	handle_signals(t_job job)
{
	signal(SIGQUIT, SIG_IGN);
	if (job == WAITING_INPUT)
		signal(SIGINT, handle_interrupt_main);
	else if (job == EXECUTING_HDOC)
		signal(SIGINT, handle_interrupt_heredocs);
	else if (job == EXECUTING_CMD)
		signal(SIGINT, handle_interrupt_child);
	else
		signal(SIGINT, SIG_IGN);
}
