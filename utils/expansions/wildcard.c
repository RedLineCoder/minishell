/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:29 by moztop            #+#    #+#             */
/*   Updated: 2024/11/17 11:46:27 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_wld_size(t_list *explst, int index, char *arg, int increment)
{
	t_write *const	wrt = &(t_write){0};

	wrt->e_i = 1;
	wrt->a_i = index;
	while (arg[wrt->a_i])
	{
		if (track_quotes(wrt, explst, arg))
		{
			wrt->a_i += increment;
			continue ;
		}
		if (!wrt->qc && arg[wrt->a_i] == '*')
			break ;
		wrt->a_i += increment;
		wrt->e_i++;
	}
	return (wrt->e_i * (arg[wrt->a_i] != 0));
}

int	ft_patterncmp(t_list *explst, t_write *wrt, char *arg, char *file)
{
	while (arg[wrt->a_i] || file[wrt->e_i])
	{
		if (track_quotes(wrt, explst, arg))
		{
			wrt->a_i++;
			continue ;
		}
		if (!wrt->qc && arg[wrt->a_i] == '*')
			break ;
		if (arg[wrt->a_i] != file[wrt->e_i])
		{
			wrt->qc = 0;
			return (1);
		}
		wrt->a_i++;
		wrt->e_i++;
	}
	return (0);
}

void	check_mid_pattern(t_list *explst, t_pattern *ptrn, t_write *wrt)
{
	int					temparg;
	int					tempfile;
	int					size;

	size = get_wld_size(explst, wrt->a_i, ptrn->arg, 1);
	while (!ptrn->diff && ptrn->file[wrt->e_i + ptrn->e_size + size - 2])
	{
		while (ptrn->arg[wrt->a_i] == '*')
			wrt->a_i++;
		if (!size)
			break ;
		while (ptrn->file[wrt->e_i + ptrn->e_size + size - 2])
		{
			temparg = wrt->a_i;
			tempfile = wrt->e_i;
			ptrn->diff = ft_patterncmp(explst, wrt, ptrn->arg, ptrn->file);
			if (!ptrn->diff)
				break ;
			wrt->a_i = temparg;
			wrt->e_i = tempfile + 1;
		}
		size = get_wld_size(explst, wrt->a_i, ptrn->arg, 1);
	}
}

int	check_pattern(t_list *explst, char *arg, char *file)
{
	t_pattern *const	ptrn = &(t_pattern){0};
	t_write *const		wrt = &(t_write){0};

	ptrn->arg = arg;
	ptrn->file = file;
	ptrn->e_size = get_wld_size(explst,
			ft_strlen(ptrn->arg) - 1, ptrn->arg, -1) - 1;
	ptrn->s_size = get_wld_size(explst, 0, ptrn->arg, 1) - 1;
	if ((!ptrn->s_size && ptrn->file[0] == '.')
		|| (ptrn->e_size + ptrn->s_size) > (int)ft_strlen(ptrn->file))
		ptrn->diff = 1;
	if (ptrn->s_size && !ptrn->diff)
		ptrn->diff = ft_patterncmp(explst, wrt, ptrn->arg, ptrn->file);
	check_mid_pattern(explst, ptrn, wrt);
	if (ptrn->arg[wrt->a_i] && !ptrn->file[wrt->e_i])
		ptrn->diff = 1;
	if (!ptrn->diff && ptrn->arg[wrt->a_i])
	{
		wrt->e_i = ft_strlen(ptrn->file) - ptrn->e_size;
		ptrn->diff = ft_patterncmp(explst, wrt, ptrn->arg, ptrn->file);
	}
	return (ptrn->diff);
}

int	expand_wildcard(t_list **expanded, t_list *explst, char *arg)
{
	DIR *const		current_dir = opendir(".");
	struct dirent	*dp;
	char			*dirname;
	int				count;

	if (!get_wld_size(explst, 0, arg, 1))
		return (closedir(current_dir), 0);
	count = 0;
	dp = readdir(current_dir);
	while (dp)
	{
		if (!check_pattern(explst, arg, dp->d_name))
		{
			dirname = ft_strdup(dp->d_name);
			if (!lst_addback_content(expanded, dirname))
				return (free(dirname), -1);
			count++;
		}
		dp = readdir(current_dir);
	}
	closedir(current_dir);
	return (count);
}
