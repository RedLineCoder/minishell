/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:29 by moztop            #+#    #+#             */
/*   Updated: 2024/10/11 14:28:55 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_wildcard(t_list *explst, char *arg)
{
	int	qs;
	int	qd;

	qs = 0;
	qd = 0;
	while (*arg)
	{
		if (*arg == '\'' && !qd && !is_expanded(explst, *arg))
			qs = !qs;
		if (*arg == '"' && !qs && !is_expanded(explst, *arg))
			qd = !qd;
		if (!(qd || qs) && *arg == '*')
			return (1);
		arg++;
	}
	return (0);
}

int	ft_patterncmp(t_list *explst, char **arg, char **file)
{
	int	qs;
	int	qd;

	qs = 0;
	qd = 0;
	while (**arg && **file)
	{
		if (**arg == '\'' && !qd && !is_expanded(explst, **arg))
			qs = !qs;
		if (**arg == '"' && !qs && !is_expanded(explst, **arg))
			qd = !qd;
		if ((!qd && **arg == '\'') || (!qs && **arg == '"'))
		{
			(*arg)++;
			continue ;
		}
		if (!(qd || qs) && *(*arg + 1) == '*')
			break ;
		if (**arg != **file)
			break ;
		(*arg)++;
		(*file)++;
	}
	return (**arg - **file);
}

int	check_pattern(t_list *explst, char *arg, char *file)
{
	int		diff;

	diff = 0;
	diff = ft_patterncmp(explst, &arg, &file);
	while (*arg && !diff)
	{
		while (*arg == '*')
			arg++;
		while (*file)
		{
			if (*arg == *file)
			{
				diff = ft_patterncmp(explst, &arg, &file);
				break ;
			}
			file++;
		}
	}
	return (diff);
}

int	expand_wildcard(t_list **expanded, t_list *explst, char *arg)
{
	DIR *const		current_dir = opendir(".");
	struct dirent	*dp;
	char			*dirname;
	int				count;

	if (!is_wildcard(explst, arg))
		return (0);
	count = 0;
	dp = readdir(current_dir);
	while (dp)
	{
		if (!check_pattern(explst, arg, dp->d_name))
		{
			dirname = ft_strdup(dp->d_name);
			if (!dirname)
				return (-1);
			if (!lst_addback_content(expanded, dirname))
				return (-1);
			count++;
		}
		dp = readdir(current_dir);
	}
	closedir(current_dir);
	return (count);
}
