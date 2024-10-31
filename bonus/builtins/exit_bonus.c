/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 06:55:00 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/31 15:58:52 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_bonus.h"

int	is_valid_status(char *str)
{
	if (!*str)
		return (false);
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
	int		exit_code;

	msh->exit_flag = true;
	exit_code = EXIT_SUCCESS;
	if (args_size >= 2)
	{
		status = args[1];
		if (!is_valid_status(status))
			exit_code = mini_panic("exit", ERR_EXIT_NUM_REQUIRED,
					EXIT_NUM_REQUIRED);
		else if (args_size >= 3)
			exit_code = mini_panic("exit", ERR_TOO_MANY_ARG, EXIT_FAILURE);
		else
			exit_code = ft_atoi(status);
	}
	return (exit_code);
}
