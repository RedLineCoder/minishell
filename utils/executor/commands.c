/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:38:09 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/21 22:09:21 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**get_env_arr(t_list *mshenv)
{
	int			i;
	char		**env;
	t_env		*node;
	const int	size = ft_lstsize(mshenv);

	env = ft_calloc(sizeof(char *), size + 1);
	if (!env)
		return (NULL);
	i = -1;
	while (mshenv)
	{
		node = mshenv->content;
		if (!node->pair)
		{
			mshenv = mshenv->next;
			continue ;
		}
		env[++i] = ft_strdup(node->key);
		if (!env[i] || !str_append(&env[i], "=")
			|| (node->pair && !str_append(&env[i], node->pair)))
			free(env[i--]);
		mshenv = mshenv->next;
	}
	return (env);
}

char	**get_args_arr(t_list *arglist)
{
	int			i;
	int			len;
	char		**args;
	t_list		*temp;

	len = 0;
	temp = arglist;
	while (temp)
	{
		if (ft_strlen(temp->content))
			len++;
		temp = temp->next;
	}
	args = ft_calloc(sizeof(char *), len + 1);
	if (!args)
		return (NULL);
	i = 0;
	while (arglist)
	{
		if (ft_strlen(arglist->content))
			args[i++] = (char *)arglist->content;
		arglist = arglist->next;
	}
	return (args);
}

int	check_executable(char *command, char *path)
{
	t_stat			file;

	if (!path)
	{
		if (*command == '.' || *command == '/')
			return (mini_panic(command, NULL, EXIT_CMD_NOTFOUND));
		return (mini_panic(command, ERR_CMD_NOTFOUND, EXIT_CMD_NOTFOUND));
	}
	else if (access(path, X_OK))
		return (mini_panic(command, NULL, EXIT_CMD_NOTEXECUTABLE));
	else if (stat(path, &file))
		return (mini_panic(command, NULL, EXIT_FAILURE));
	else if (S_ISDIR(file.st_mode))
		return (mini_panic(command, ERR_CMD_ISDIR, EXIT_CMD_NOTEXECUTABLE));
	return (EXIT_SUCCESS);
}

char	*get_executable_path(char *command, t_list *env)
{
	int			i;
	char		**paths;
	char		*path;
	const int	relative_path = *command == '.' || *command == '/';
	char *const	pathenv = get_env(env, "PATH");

	if (relative_path && !access(command, F_OK))
		return (command);
	if (!pathenv)
		return (NULL);
	paths = ft_split(pathenv, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], "/");
		if (!path
			|| (str_append(&path, command) && !access(path, F_OK)))
			break ;
		free(path);
		path = NULL;
	}
	return (free_string_array(paths), path);
}

int	run_command(char *command, char **args, t_list	*env)
{
	int				status;
	char			*path;
	char **const	envarr = get_env_arr(env);

	if (!envarr)
		return (mini_panic(command, NULL, EXIT_FAILURE));
	path = get_executable_path(command, env);
	status = check_executable(command, path);
	if (status == EXIT_SUCCESS)
	{
		execve(path, args, envarr);
		status = mini_panic(command, NULL, EXIT_FAILURE);
	}
	free(path);
	free_string_array(envarr);
	return (status);
}