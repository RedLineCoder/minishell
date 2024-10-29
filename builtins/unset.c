/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:08:33 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/29 10:08:49 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_unset(int args_size, char **args, t_msh *msh)
{
	int	i;

	i = 0;
	while (++i < args_size)
		unset_env(&msh->env, args[i]);
	return (EXIT_SUCCESS);
}
