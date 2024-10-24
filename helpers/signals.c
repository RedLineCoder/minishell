/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:16:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/24 21:48:49 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_interrupt(int signal)
{
    (void)signal;
    if (job == EXECUTING_CMD)
        exit(130);
    else if (job != EXECUTING_HDOC && job != WAITING_INPUT)
        return ;
    if (job == EXECUTING_HDOC)
    {
        ft_putstr_fd("\n", STDOUT_FILENO);
    }   
    else
    {
        rl_on_new_line();
        rl_replace_line("", 0);
        ft_putstr_fd("\n", STDOUT_FILENO);
        rl_redisplay();
    }
}

void    handle_signals()
{
    signal(SIGINT, handle_interrupt);
}