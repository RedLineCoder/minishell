/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 06:59:01 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/25 19:45:40 by emyildir         ###   ########.fr       */
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
		return (mini_panic("pwd", NULL, EXIT_FAILURE));
	ft_putendl_fd(path, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
