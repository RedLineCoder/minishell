/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 07:03:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/21 22:33:47 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(int args_size, char **args, t_msh *msh)
{
	int		newline_flag;
	int		i;
	int		j;

	(void)msh;
	newline_flag = true;
	i = 0;
	while (++i < args_size)
	{
		if (i == 1 && !ft_strncmp(args[1], "-", 1))
		{
			j = 0;
			while (args[1][++j] == 'n')
				;
			newline_flag = args[1][j];
			if (!newline_flag)
				continue ;
		}
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (i != args_size - 1)
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (newline_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
