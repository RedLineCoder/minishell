/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 06:55:00 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/06 17:50:39 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_status(char *str)
{
	while (*str++)
		if (!ft_isdigit(*str) || (*str || (*str >= 9 && *str <=13)))
			return (false);
	return (true);
}

int	builtin_exit(int args_size, char **args, t_msh *msh)
{
	char	*status;
	
	(void)msh;	
	printf("exit\n");
	status = EXIT_SUCCESS;
	if (args_size > 2)
		mini_panic("exit", "too many arguments\n", true, 255);
	if (args_size == 2)
	{
		status = args[1];
		if (!is_valid_status(status))
			mini_panic("exit", "numeric argument required\n", true, 255);	
		exit(ft_atoi(status));
	}
	exit(EXIT_SUCCESS);
}