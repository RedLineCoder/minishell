/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 03:47:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/08/26 05:11:15 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command(char *command, char **args, char **env)
{
	char	*path;
	bool	args_created;

	path = get_path(command);
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
