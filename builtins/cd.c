/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 02:41:44 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/07 16:57:00 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_cd(int args_size, char **args, t_msh *msh)
{
	char	*path;
	char	*err;

	if (args_size == 1)
	{
		path = get_env(msh->env, "HOME");
		err = "HOME not set\n";
	}
	else
	{
		path = args[1];
		err = NULL;
	}
	if (chdir(path) == -1)
		return (mini_panic("cd", err, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}
