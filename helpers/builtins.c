/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 14:44:50 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/15 14:58:45 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_index(char *command)
{
	if (!ft_strncmp(command, "echo", 5))	
		return (BUILTIN_ECHO);
	else if (!ft_strncmp(command, "cd", 5))	
		return (BUILTIN_CD);
	else if (!ft_strncmp(command, "pwd", 5))	
		return (BUILTIN_PWD);
	else if (!ft_strncmp(command, "export", 5))	
		return (BULTIN_EXPORT);
	else if (!ft_strncmp(command, "unset", 5))	
		return (BUILTIN_UNSET);
	else if (!ft_strncmp(command, "env", 5))	
		return (BUILTIN_ENV);
	else if (!ft_strncmp(command, "exit", 5))	
		return (BUILTIN_EXIT);
	return -1;
}

int	handle_builtin(char *command, char **args, t_msh *msh)
{
	void	(*funcs[8])(char **args, t_msh *msh);
	
	memset(funcs, 0, sizeof(funcs));
	funcs[BUILTIN_ECHO] =
}