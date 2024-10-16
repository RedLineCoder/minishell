/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 08:55:37 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/13 19:22:14 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(char *str)
{
	if (!*str)
		return (false);
	if (*str == '-' || *str == '_')
		str++;
	while (*str)
	{
		if (ft_isdigit(*str) || *str == ' '
			|| (*str >= 9 && *str <= 13) || !ft_isalpha(*str))
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
			return (mini_panic("export", "not a valid identifier\n", \
			EXIT_FAILURE));
		if (!set_env(&msh->env, key, pair))
			return (mini_panic("export", "malloc error\n", \
			EXIT_FAILURE));
	}
	return (EXIT_SUCCESS);
}
