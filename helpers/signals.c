/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:16:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/23 10:54:24 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_interrupt(int something)
{
    (void)something;
    if (job == EXECUTING_CMD)
        exit(130);
    else if (job != EXECUTING_HDOC && job != WAITING_INPUT)
        return ;
    if (job == EXECUTING_HDOC)
    {
        job = NOTHING;
        write(STDOUT_FILENO, "\033[A", 3);
        ioctl(0, TIOCSTI, "\n");
    }
    else
    {
         rl_on_new_line();
        rl_replace_line("", 0);
        ft_putstr_fd("\n", STDOUT_FILENO);
        rl_redisplay();
    }
}

void handle_quit(int something)
{
    (void)something;
    printf("asdasd\n");
    exit(2);
}

void    handle_signals()
{
    signal(SIGINT, handle_interrupt);
    signal(SIGQUIT, handle_quit);
}