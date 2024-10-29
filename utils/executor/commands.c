/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:38:09 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/29 10:05:53 by moztop           ###   ########.fr       */
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
	int			len;
	char		*expanded;
	char		**args;
	t_list		*temp;

	while (arglist)
	{
		expanded = expand_dollar(arglist->content, NULL, msh);
		if (ft_strlen(expanded))
		{
			free(expanded);
			break ;
		}
		free(expanded);
		arglist = arglist->next;
	}
	temp = expander(arglist, msh);
	len = ft_lstsize(temp);
	args = ft_calloc(sizeof(char *), len + 1);
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

	if ((relative_path || !pathenv) && !access(command, F_OK))
		return (command);
	if (!pathenv || relative_path)
		return (NULL);
	paths = ft_split(pathenv, ':');
	if (!paths)
		return (NULL);
	path = NULL;
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
