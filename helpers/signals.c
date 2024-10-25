/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:16:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/25 14:55:36 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_interrupt(int signal)
{
    (void)signal;
    if (job == WAITING_HDOC)
        ft_putstr_fd("\n", STDOUT_FILENO);
    else if (job == WAITING_INPUT)
    {
        rl_on_new_line();
        rl_replace_line("", 0);
        ft_putstr_fd("\n", STDOUT_FILENO);
        rl_redisplay();
    }
    else 
        exit(EXIT_SIGINT);
}

void    handle_signals()
{
    signal(SIGINT, handle_interrupt);
}