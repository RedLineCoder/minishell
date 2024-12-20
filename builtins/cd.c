/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 02:41:44 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/26 15:05:47 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_cd(int args_size, char **args, t_msh *msh)
{
	char	*path;
	char	*err;

	err = NULL;
	path = NULL;
	if (args_size == 1 && !path)
	{
		path = get_env(msh->env, "HOME");
		if (!path)
			err = ERR_CD_HOME_NOT_SET;
	}
	else if (args_size == 2)
		path = args[1];
	else
		err = ERR_TOO_MANY_ARG;
	if (err || chdir(path) == -1)
		return (mini_panic("cd", err, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}
