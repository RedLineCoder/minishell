/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:20:28 by emyildir          #+#    #+#             */
/*   Updated: 2024/08/27 18:25:30 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_user(void)
{
	int		p[2];
	pid_t	pid;
	char	*user;

	if (pipe(p) < 0)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (close(p[0]), close(p[1]), NULL);
	else if (!pid && dup2(p[1], STDOUT_FILENO) == STDOUT_FILENO && !close(p[0]))
		execute_command("whoami", NULL, NULL);
	waitpid(pid, 0, 0);
	close(p[1]);
	user = get_next_line(p[0]);
	close(p[0]);
	if (!user)
		return (0);
	user[ft_strlen(user) - 1] = '\0';
	return (user);
}
