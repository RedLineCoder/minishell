/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 06:59:01 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/05 07:10:36 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(int args_size, char **args, t_msh *msh)
{
	char	path[PATH_MAX];
	
	(void)args_size;
	(void)args;
	(void)msh;
	if (!getcwd(path, PATH_MAX))
	{
		mini_panic("pwd", NULL, false, -1);
		return (EXIT_FAILURE);
	}
	else
		printf("%s\n", path);
	return(EXIT_SUCCESS);
}