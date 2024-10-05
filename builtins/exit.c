/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 06:55:00 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/05 06:58:25 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_exit(int args_size, char **args, t_msh *msh)
{
	(void)args_size;
	(void)args;
	(void)msh;
	printf("exit\n");
	exit(EXIT_SUCCESS);
}