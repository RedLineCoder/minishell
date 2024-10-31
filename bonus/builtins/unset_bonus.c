/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:08:33 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/31 15:58:46 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_bonus.h"

int	builtin_unset(int args_size, char **args, t_msh *msh)
{
	int	i;

	i = 0;
	while (++i < args_size)
		unset_env(&msh->env, args[i]);
	return (EXIT_SUCCESS);
}
