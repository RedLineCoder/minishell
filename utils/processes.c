/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 12:25:30 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/13 15:43:09 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t	create_child(int pipe[2], int fd)
{
	pid_t const	pid = fork();

	if (pid)
		return (pid);
	child = 1;
	if (pipe)
	{
		dup2(pipe[fd], fd);
		close_pipe(pipe);
	}
	return (0);
}