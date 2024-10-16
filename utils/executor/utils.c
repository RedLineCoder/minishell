/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 08:08:16 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/16 18:42:15 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

int	execute_command(char *command, char **args, t_list	*env)
{
	char			*path;
	t_stat			file;
	char **const	envarr = get_env_arr(env);

	if (!envarr)
		return (mini_panic(command, NULL, EXIT_FAILURE));
	path = get_cmd_path(command, env);
	if (!path)
		return (free_string_array(envarr),\
		mini_panic(command, NULL, EXIT_CMD_NOTFOUND));
	else if (stat(path, &file))
		return (free_string_array(envarr), free(path),\
		mini_panic(command, NULL, EXIT_FAILURE)); 
	else if (S_ISDIR(file.st_mode))
		return (free_string_array(envarr), free(path),\
		mini_panic(command, ERR_CMD_ISDIR, EXIT_CMD_NOTEXECUTABLE));
	execve(path, args, envarr);
	free(path);
	free_string_array(envarr);
	return (mini_panic(command, NULL, EXIT_FAILURE));
}

char	*get_cmd_path(char *command, t_list *env)
{
	char *const	pathenv = get_env(env, "PATH");
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
	if (!path && !access(command, F_OK | X_OK))
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
		args[i++] = (char *)arglist->content;
		arglist = arglist->next;
	}
	return (args);
}
