/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 08:55:37 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/31 15:58:50 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_bonus.h"

int	is_valid_identifier(char *str)
{
	if (!*str || ft_isdigit(*str))
		return (false);
	while (*str)
	{
		if (*str == ' '
			|| (*str >= 9 && *str <= 13)
			|| (!ft_isalnum(*str) && *str != '_'))
			return (false);
		str++;
	}
	return (true);
}

int	builtin_export(int args_size, char **args, t_msh *msh)
{
	char	*key;
	char	*pair;

	if (args_size == 1)
	{
		print_env(msh->env, true, false);
		return (EXIT_SUCCESS);
	}
	args++;
	while (--args_size > 0)
	{
		key = *args++;
		pair = ft_strchr(key, '=');
		if (pair)
			*(pair++) = '\0';
		if (!is_valid_identifier(key))
			return (mini_panic("export", ERR_INVALID_IDENTIFIER, \
			EXIT_INVALID_IDENTIFIER));
		if (!set_env(&msh->env, key, pair))
			return (mini_panic("export", NULL, \
			EXIT_FAILURE));
	}
	return (EXIT_SUCCESS);
}
