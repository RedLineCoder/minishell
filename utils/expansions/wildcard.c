/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:29 by moztop            #+#    #+#             */
/*   Updated: 2024/10/09 14:10:28 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* int	is_pattern(char *arg)
{
	int	quoted;

	quoted = 0;
	while (*arg)
	{
		if (str_include(QUOTES, *arg))
			quoted = !quoted;
		if (*arg == '*' && !quoted)
			return (1);
		arg++;
	}
	return (0);
} */

int	ft_dstrncmp(char **arg, char **file)
{
	while (**arg == **file && **arg && **file)
	{
		(*arg)++;
		(*file)++;
	}
	return (**arg - **file);
	// Solve star problem
}

int	check_pattern(t_list *explst, char *arg, char *file)
{
	int		diff;
	char	qs;
	char	qd;

	diff = 0;
	qs = 0;
	qd = 0;
	while (*arg && !diff)
	{
		while (*arg == '*')
			arg++;
		while (*file)
		{
			if (*arg == '"' && !qs && !is_expanded(explst, *arg))
				qd = !qd;
			if (*arg == '\'' && !qd && !is_expanded(explst, *arg))
				qs = !qs;
			if (*arg == *file)
			{
				diff = ft_dstrncmp(&arg, &file);
				break ;
			}
			file++;
		}
	}
	return (diff);
}
