/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:38:09 by emyildir          #+#    #+#             */
/*   Updated: 2024/11/15 19:36:14 by emyildir         ###   ########.fr       */
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

char	**get_args_arr(t_list *arglist, t_msh *msh)
{
	int			i;
	char		*expanded;
	char		**args;
	t_list		*temp;

	while (arglist)
	{
		expanded = expand_dollar(arglist->content, NULL, msh);
		if (ft_strlen(expanded) && (free(expanded), 1))
			break ;
		free(expanded);
		arglist = arglist->next;
	}
	temp = expander(arglist, msh);
	args = ft_calloc(sizeof(char *), ft_lstsize(temp) + 1);
	if (!args)
		return (ft_lstclear(&temp, free), NULL);
	i = 0;
	arglist = temp;
	while (temp)
	{
		args[i++] = (char *)temp->content;
		temp = temp->next;
	}
	return (free_list(arglist), args);
}

int	check_executable(char *command, char *path)
{
	t_stat			file;

	if (!path || ft_strlen(command) == 0)
	{
		if (ft_strchr(command, '/'))
			return (mini_panic(command, ERR_FILE_NOTFOUND, EXIT_CMD_NOTFOUND));
		return (mini_panic(command, ERR_CMD_NOTFOUND, EXIT_CMD_NOTFOUND));
	}
	else if (!ft_strncmp(command, ".", 2))
		return (mini_panic(command, ERR_CMD_ARGREQ, EXIT_ARG_REQUIRED));
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
	int				i;
	char			*path;
	const int		relative_path = ft_strchr(command, '/') && 1;
	char *const		pathenv = get_env(env, "PATH");
	char **const	paths = ft_split(pathenv, ':');

	path = NULL;
	if (!relative_path)
	{
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
	}
	else if (!access(command, F_OK))
		return (ft_strdup(command));
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
		free(path);
		status = mini_panic(command, NULL, EXIT_FAILURE);
	}
	free(path);
	free_string_array(envarr);
	return (status);
}
