/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 12:25:30 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/25 14:46:52 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t	create_child(int pipe[2], int fd)
{
	pid_t const	pid = fork();

	if (pid)
		return (pid);
	if (pipe)
	{
		dup2(pipe[fd], fd);
		close_pipe(pipe);
	}
	return (0);
}

int	wait_child_processes(int pid)
{
	int		status;

	status = 0;
	if (pid)
		waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	if (!pid)
		return (0);
	if (!WIFEXITED(status))
		return (130);
	return (WEXITSTATUS(status));
}