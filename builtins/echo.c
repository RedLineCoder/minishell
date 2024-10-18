/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 07:03:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/18 17:44:07 by emyildir         ###   ########.fr       */
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
			if (!args[1][j])
				newline_flag = false;
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
