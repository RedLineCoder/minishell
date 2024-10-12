/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:29 by moztop            #+#    #+#             */
/*   Updated: 2024/10/12 14:25:22 by moztop           ###   ########.fr       */
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
		if (*arg == '\'' && !qd && !is_expanded(explst, arg))
			qs = !qs;
		if (*arg == '"' && !qs && !is_expanded(explst, arg))
			qd = !qd;
		if (!(qd || qs) && *arg == '*')
			return (1);
		arg++;
	}
	return (0);
}

int	ft_patterncmp(t_list *explst, char **arg, char **file)
{
	int	diff;
	int	qs;
	int	qd;

	qs = 0;
	qd = 0;
	while (**arg && **file)
	{
		if (**arg == '\'' && !qd && !is_expanded(explst, *arg))
			qs = !qs;
		if (**arg == '"' && !qs && !is_expanded(explst, *arg))
			qd = !qd;
		if ((!qd && **arg == '\'') || (!qs && **arg == '"'))
		{
			(*arg)++;
			continue ;
		}
		if ((!(qd || qs) && *(*arg + 1) == '*') || **arg != **file)
			break ;
		(*arg)++;
		(*file)++;
	}
	diff = **arg - **file;
	return ((*arg)++, (*file)++, diff);
}

int	check_pattern(t_list *explst, char *arg, char *file)
{
	int		diff;

	diff = 0;
	while (*arg && !diff)
	{
		while (*arg == '*')
			arg++;
		if (is_wildcard(explst, arg))
		{
			while (*arg != *file)
				file++;
			diff = ft_patterncmp(explst, &arg, &file);
			if (diff)
				break ;
		}
		else
			break;
	}
	file += ft_strlen(file) - ft_strlen(arg);
	if (!diff)
		diff = ft_patterncmp(explst, &arg, &file);
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
