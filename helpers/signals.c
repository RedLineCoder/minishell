/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:16:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/29 17:41:16 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
int a = 0;

void	handle_interrupt_main(int sig)
{
    (void)sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_interrupt_child(int signal)
{
	(void)signal;
    ft_putstr_fd("\n", STDOUT_FILENO);
	exit(130);
}

void    set_termflags()
{
    t_termios   termios;

    if (tcgetattr(STDIN_FILENO, &termios) && mini_panic(NULL, NULL, -1))
        return ;
    termios.c_lflag |= ECHOCTL;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios))
        mini_panic(NULL, NULL, -1);
}

void	handle_signals(t_job job)
{
    signal(SIGQUIT, SIG_IGN);
	if (job == WAITING_INPUT)
		signal(SIGINT, handle_interrupt_main);
	else if (job == EXECUTING_CMD || job == EXECUTING_HDOC)
		signal(SIGINT, handle_interrupt_child);
	else
		signal(SIGINT, SIG_IGN);
}
