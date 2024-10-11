/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 07:03:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/06 20:02:49 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(int args_size, char **args, t_msh *msh)
{
	int		newline_flag;
	int		i;

	(void)msh;
	newline_flag = true;
	i = 0;
	while (++i < args_size)
	{
		if (i == 1 && !ft_strncmp(args[i], "-n", 3))
			newline_flag = false;
		else
		{
			printf("%s", args[i]);
			if (i != args_size - 1)
				printf(" ");
		}
	}
	if (newline_flag)
		printf("\n");
	return (EXIT_SUCCESS);
}
