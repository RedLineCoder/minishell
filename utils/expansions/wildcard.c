/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:29 by moztop            #+#    #+#             */
/*   Updated: 2024/10/15 15:34:50 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_wld_size(t_list *explst, char *arg, int increment)
{
	t_write *const wrt = &(t_write){0};
	wrt->e_i = 1;
	while (arg[wrt->a_i])
	{
		if (arg[wrt->a_i] == '\'' && !wrt->qd && !is_expanded(explst,
			wrt->a_i))
			wrt->qs = !wrt->qs;
		if (arg[wrt->a_i] == '"' && !wrt->qs && !is_expanded(explst,
				wrt->a_i))
			wrt->qs = !wrt->qs;
		if ((!wrt->qs && arg[wrt->a_i] == '\'') || (!wrt->qs
				&& arg[wrt->a_i] == '"'))
		{
			wrt->a_i++;
			continue ;
		}
		if (!(wrt->qd || wrt->qs) && arg[wrt->a_i] == '*')
			break ;
		wrt->a_i += increment;
		wrt->e_i++;
	}
	return (wrt->e_i * (arg[wrt->a_i] != 0));
}

int	ft_patterncmp(t_list *explst, char **arg, char **file)
{
	t_write *const wrt = &(t_write){0};
	while (**arg || **file)
	{
		if (**arg == '\'' && !wrt->qd && !is_expanded(explst, wrt->a_i))
			wrt->qs = !wrt->qs;
		if (**arg == '"' && !wrt->qs && !is_expanded(explst, wrt->a_i))
			wrt->qd = !wrt->qd;
		if ((!wrt->qd && **arg == '\'') || (!wrt->qs && **arg == '"'))
		{
			(*arg)++;
			wrt->a_i++;
			continue ;
		}
		if ((!(wrt->qd || wrt->qs) && **arg == '*'))
			break ;
		if (**arg != **file)
			return (1);
		wrt->a_i++;
		(*arg)++;
		(*file)++;
	}
	return (0);
}

int	check_mid_pattern(t_list *explst, char *arg)

int	check_pattern(t_list *explst, char *arg, char *file)
{
	t_pattern *const	ptrn = &(t_pattern){0};

	ptrn->e_size = get_wld_size(explst, arg + ft_strlen(arg) - 1, -1) - 1;
	ptrn->s_size = get_wld_size(explst, arg, 1) - 1;
	if ((!ptrn->s_size && *file == '.') || (ptrn->e_size
			+ ptrn->s_size) > (int)ft_strlen(file))
		ptrn->diff = 1;
	if (ptrn->s_size && !ptrn->diff)
		ptrn->diff = ft_patterncmp(explst, &arg, &file);
	while (!ptrn->diff)
	{
		while (*arg == '*')
			arg++;
		if (!get_wld_size(explst, arg, 1))
			break ;
		while (*arg != *file && *(file + ptrn->e_size))
			file++;
		ptrn->diff = ft_patterncmp(explst, &arg, &file);
	}
	if (!ptrn->diff && *arg)
	{
		file += ft_strlen(file) - ptrn->e_size;
		ptrn->diff = ft_patterncmp(explst, &arg, &file);
	}
	return (ptrn->diff);
}

int	expand_wildcard(t_list **expanded, t_list *explst, char *arg)
{
	struct dirent	*dp;
	char			*dirname;
	int				count;

	DIR *const current_dir = opendir(".");
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
