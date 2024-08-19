/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:20:28 by emyildir          #+#    #+#             */
/*   Updated: 2024/08/19 23:21:12 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path(char *command)
{
	char	*const	pathenv = getenv("PATH");
	char			**paths;
	char			*path;
	size_t			i;

	if (!pathenv)
		return (NULL);
	paths = ft_split(pathenv, ':');
	if (!paths)
		return (NULL);	
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i++], "/");
		str_append(&path, command);
		if (!access(path, F_OK))
			break ;
		free(path);
	}
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (path);
}

char	*get_user(t_msh *msh)
{
	pid_t	pid;
	char	buffer[] = {'\0'};
	int		bytes_read;
	int		p[2];
	char	**splitted;

	if (pipe(p) < 0)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (close(p[0]), close(p[1]), NULL);
	else if (!pid)
	{
		dup2(p[1], STDOUT_FILENO);
		close(p[0]);
		splitted = ft_split("somearg", ' ');
		if (execve(get_path("who"), splitted, msh->env) == -1)
			exit(0);
	}	
	waitpid(pid, 0, -1);
	close(p[1]);
	bytes_read = read(p[0], buffer, 100);
	close(p[0]);
	buffer[bytes_read] = '\0';
	printf("%s\n", buffer);
	splitted = ft_split(buffer, ' ');
	return (*splitted);
}