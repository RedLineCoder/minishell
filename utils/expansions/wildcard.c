/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:29 by moztop            #+#    #+#             */
/*   Updated: 2024/10/06 19:57:55 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_wildsize(char *arg)
{
	int	size;

	size = 0;
	while (*arg && *arg != '*')
	{
		arg++;
		size++;
	}
	if (arg != '*')
		return (-size);
	return (size);
}

int	is_pattern(char *arg)
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
}

int	ft_dstrncmp(char **arg, char **file, int n)
{
	if (n > 0)
	{
		while (**arg == **file && n)
		{
			(*arg)++;
			(*file)++;
			n--;
		}
		return (**arg - **file);
	}
	if (n < 0)
	{
		while (**file)
			file++;
		while (**arg == **file && n)
		{
			(*arg)--;
			(*file)--;
			n++;
		}
		return (**arg - **file);
	}
	return (0);
}

int	check_pattern(char *arg, char *file)
{
	int	size;
	int	diff;

	diff = 0;
	while (*arg && !diff)
	{
		while (*arg == '*')
			arg++;
		size = get_wildsize(arg);
		while (*file && size > 0)
		{
			if (*arg == *file)
			{
				diff = ft_dstrncmp(&arg, &file, size);
				break ;
			}
			file++;
		}
	}
	if (diff)
		return (diff);
	if (size < 0)
		diff = ft_dstrncmp(&arg, &file, size);
	return (diff);
}

t_list	*file_expansion(char *arg)
{
}
