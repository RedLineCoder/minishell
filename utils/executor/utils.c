/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:08:16 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/05 02:38:00 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

int	wait_child_processes(int pid)
{
	int		status;

	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	return (status >> 8);
}

void	execute_command(char *command, char **args, char **env)
{
	int		args_created;
	char	*path;
	char	*err;
	t_stat	file;
	
	err = NULL;
	path = get_cmd_path(command);
	if (!path)
		err = "command not found\n";
	else if (stat(path, &file))
		err = "error reading file information\n";
	else if (S_ISDIR(file.st_mode))
		err = "is a directory\n";
	if (err)
		mini_panic(args[0], err, true, EXIT_FAILURE);
	args_created = false;
	if (!args)
	{
		args = ft_split(command, ' ');
		args_created = true;
	}
	execve(path, args, env);
	if (args_created)
		free_string_array(args);
	exit(EXIT_FAILURE);
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
		if (str_append(&path, command) && !access(path, F_OK))
			break ;
		free(path);
		path = NULL;
	}
	free_string_array(paths);
	if (!access(command, F_OK | X_OK))
		return (command);
	return (path);
}

char	**get_args_arr(t_list *arglist)
{
	int			i;
	char		**args;
	const int	len = ft_lstsize(arglist);

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
