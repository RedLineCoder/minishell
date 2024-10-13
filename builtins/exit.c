/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 06:55:00 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/13 19:12:23 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_status(char *str)
{
	while (*str && (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	builtin_exit(int args_size, char **args, t_msh *msh)
{
	char	*status;

	status = EXIT_SUCCESS;
	if (args_size > 2)
		return (mini_panic("exit", "too many arguments\n", EXIT_FAILURE));
	if (args_size == 2)
	{
		status = args[1];
		if (!is_valid_status(status))
			return (mini_panic("exit", "numeric argument required\n", 255));
		msh->exit_flag = true;
		return(ft_atoi(status));
	}
	msh->exit_flag = true;
	return (EXIT_SUCCESS);
}

