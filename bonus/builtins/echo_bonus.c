/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 07:03:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/31 15:58:57 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_bonus.h"

int	is_nonewline_flag(char *arg)
{
	int	i;

	i = 0;
	if (!arg[i] || arg[i++] != '-')
		return (false);
	while (arg[i] == 'n')
		i++;
	return (i > 1 && !arg[i]);
}

int	builtin_echo(int args_size, char **args, t_msh *msh)
{
	int	i;
	int	newline_flag;
	int	track_nl_flag;

	(void)msh;
	newline_flag = true;
	i = 0;
	track_nl_flag = 1;
	while (++i < args_size)
	{
		if (i == track_nl_flag && is_nonewline_flag(args[track_nl_flag]))
		{
			newline_flag = false;
			track_nl_flag++;
		}
		else
		{
			ft_putstr_fd(args[i], STDOUT_FILENO);
			if (i != args_size - 1)
				ft_putstr_fd(" ", STDOUT_FILENO);
		}
	}
	if (newline_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
