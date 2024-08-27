/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 03:47:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/08/27 18:23:29 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_cmd_path(char *command)
{
	char *const	pathenv = getenv("PATH");
	char		**paths;
	char		*path;
	size_t		i;

	if (!pathenv)
		return (NULL);
	paths = ft_split(pathenv, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i++], "/");
		if (!path)
			break ;
		str_append(&path, command);
		if (!access(path, F_OK))
			break ;
		free(path);
	}
	free_string_array(paths);
	return (path);
}

void	execute_command(char *command, char **args, char **env)
{
	char	*path;
	bool	args_created;

	path = get_cmd_path(command);
	if (!path)
		exit(EXIT_FAILURE);
	args_created = false;
	if (!args)
	{
		args = ft_split(command, ' ');
		args_created = true;
	}
	if (execve(path, args, env) == -1)
	{
		if (args_created)
			free_string_array(args);
		exit(EXIT_FAILURE);
	}
}
