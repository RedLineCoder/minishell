/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:08:16 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/29 19:28:54 by emyildir         ###   ########.fr       */
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

int	wait_child_processes(int pid)
{
	int		status;
	int		requested_status;
	int		newline_flag;

	newline_flag = false;
	if (pid)
		waitpid(pid, &requested_status, 0);
	while (wait(&status) != -1)
	{
		if (WIFSIGNALED(status))
			newline_flag = true;
	}
	if (isatty(STDOUT_FILENO) && (newline_flag 
		|| (!pid || WIFSIGNALED(requested_status))))
		ft_putchar_fd('\n', STDOUT_FILENO);
	if (!pid)
		return (0);
	if (WIFSIGNALED(requested_status))
		return (EXIT_SIGINT);
	return (WEXITSTATUS(requested_status));
}
