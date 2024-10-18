/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 07:03:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/18 10:20:21 by moztop           ###   ########.fr       */
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
		}
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
