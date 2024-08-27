/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:20:28 by emyildir          #+#    #+#             */
/*   Updated: 2024/08/26 05:07:30 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_user(void)
{
	int		p[2];
	pid_t	pid;
	char	*buffer;
	char	**splitted;

	if (pipe(p) < 0)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (close(p[0]), close(p[1]), NULL);
	else if (!pid && dup2(p[1], STDOUT_FILENO) == STDOUT_FILENO && !close(p[0]))
		execute_command("who", NULL, NULL);
	waitpid(pid, 0, 0);
	close(p[1]);
	buffer = get_next_line(p[0]);
	close(p[0]);
	if (!buffer)
		return (0);
	splitted = ft_split(buffer, ' ');
	free(buffer);
	buffer = ft_strdup(splitted[0]);
	free_string_array(splitted);
	return (buffer);
}
