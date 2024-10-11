/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:13:11 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/07 17:15:19 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_status(int args_size, char **args, t_msh *msh)
{
	(void)args_size;
	(void)args;
	printf("%d\n", msh->last_status);
	return (EXIT_SUCCESS);
}