/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:08:16 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/30 19:42:23 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

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

void	handle_sigint_output(void)
{
	if (isatty(STDOUT_FILENO))
		ft_putchar_fd('\n', STDOUT_FILENO);
}

int	get_status(int status)
{
	if (WIFSIGNALED(status))
		return (EXIT_SIGINT);
	return (WEXITSTATUS(status));
}

int	wait_child_processes(int pid)
{
	int		status;
	int		requested_status;
	int		any_interrupted;

	any_interrupted = false;
	if (pid)
		waitpid(pid, &requested_status, 0);
	any_interrupted = WIFSIGNALED(requested_status);
	while (wait(&status) != -1)
		if (!any_interrupted && WIFSIGNALED(status))
			any_interrupted = true;
	if (any_interrupted)
		handle_sigint_output();
	if (!pid)
		return (0);
	return (get_status(requested_status));
}
