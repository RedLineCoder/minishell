/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:29 by moztop            #+#    #+#             */
/*   Updated: 2024/10/13 15:18:47 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_wld_size(t_list *explst, char *arg, int increment)
{
	int	size;
	int	qs;
	int	qd;

	qs = 0;
	qd = 0;
	size = 1;
	while (*arg)
	{
		if (*arg == '\'' && !qd && !is_expanded(explst, arg))
			qs = !qs;
		if (*arg == '"' && !qs && !is_expanded(explst, arg))
			qd = !qd;
		if ((!qd && *arg == '\'') || (!qs && *arg == '"'))
		{
			arg += increment;
			continue ;
		}
		if (!(qd || qs) && *arg == '*')
			break ;
		arg += increment;
		size++;
	}
	return (size * (*arg != 0));
}

int	ft_patterncmp(t_list *explst, char **arg, char **file)
{
	int	qs;
	int	qd;

	qs = 0;
	qd = 0;
	while (**arg || **file)
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
		if ((!(qd || qs) && **arg == '*'))
			break ;
		if (**arg != **file)
			return (1);
		(*arg)++;
		(*file)++;
	}
	return (0);
}

int	check_pattern(t_list *explst, char *arg, char *file)
{
	t_pattern *const	ptrn = &(t_pattern){0};

	ptrn->end_size = get_wld_size(explst, arg + ft_strlen(arg) - 1, -1) - 1;
	ptrn->start_size = get_wld_size(explst, arg, 1) - 1;
	if (!ptrn->start_size && *file == '.')
		ptrn->diff = 1;
	if (ptrn->start_size)
		ptrn->diff = ft_patterncmp(explst, &arg, &file);
	while (!ptrn->diff)
	{
		while (*arg == '*')
			arg++;
		if (*(arg + ptrn->end_size))
		{
			while (*arg != *file && *(file + ptrn->end_size))
				file++;
			ptrn->diff = ft_patterncmp(explst, &arg, &file);
		}
		else
			break;
	}
	if (!ptrn->diff && *arg)
	{
		file += ft_strlen(file) - ptrn->end_size;
		ptrn->diff = ft_patterncmp(explst, &arg, &file);
	}
	return (ptrn->diff);
}

int	expand_wildcard(t_list **expanded, t_list *explst, char *arg)
{
	DIR *const		current_dir = opendir(".");
	struct dirent	*dp;
	char			*dirname;
	int				count;

	if (!get_wld_size(explst, arg, 1))
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
