/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:08:16 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/22 21:26:20 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	dup_io(int input_fd, int output_fd, int close_fd)
{
	close(close_fd);
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
	return (1);
}

void	execute_command(char *command, char **args, char **env)
{
	char	*path;
	bool	args_created;

	path = get_cmd_path(command);
	if (!path)
		mini_panic("Command not found.\n");
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
		path = NULL;
	}
	free_string_array(paths);
	return (path);
}

char	**get_args_arr(t_list	*arglist)
{
	int			i;
	char		**args;
	const int	len = ft_lstsize(arglist);

	if (!len)
		return (NULL);
	args = ft_calloc(sizeof(char *), len + 1);
	if (!args)
		return (NULL);
	i = 0;
	while (arglist)
	{
		args[i] = (char *)arglist->content;
		if (!args[i++])
			return (free_string_array(args), NULL);
		arglist = arglist->next;
	}
	return (args);		
}
