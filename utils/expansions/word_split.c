/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:46:35 by moztop            #+#    #+#             */
/*   Updated: 2024/10/23 16:46:37 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_explst_word(t_write *wrt, t_list *explst, t_list *explstword)
{
	int			start;

	while (wrt->e_i <= wrt->a_i)
	{
		if (is_expanded(explst, wrt->e_i))
		{
			start = wrt->e_i;
			while (is_expanded(explst, wrt->e_i))
				wrt->e_i++;
			if (!set_exptrack(&explstword, start, wrt->e_i))
				return (ft_lstclear(&explstword, free), 0);
		}
		else
			wrt->e_i++;
	}
	return (1);
}

char	*get_nextarg(t_write *wrt, t_list *explst, char *arg)
{
	char	*nextarg;

	while (arg[wrt->a_i] && str_include(SPACES, arg[wrt->a_i]))
		wrt->a_i++;
	wrt->e_i = wrt->a_i;
	while (arg[wrt->a_i] && (!str_include(SPACES, arg[wrt->a_i]
		&& !wrt->qd && !wrt->qs)))
	{
		if (arg[wrt->a_i] == '\'' && !wrt->qd && !is_expanded(explst, wrt->a_i))
			wrt->qs = !wrt->qs;
		if (arg[wrt->a_i] == '"' && !wrt->qs && !is_expanded(explst, wrt->a_i))
			wrt->qd = !wrt->qd;
		wrt->a_i++;
	}
	nextarg = ft_strndup(&arg[wrt->e_i], wrt->a_i - wrt->e_i);
	return (nextarg);
}

int	split_words(t_list *newargs, t_list *explst, char *arg)
{
	t_write *const	wrt = &(t_write){0};
	t_list	*explstword;
	char	*expanded;
	int		status;

	while (arg[wrt->a_i])
	{
		explstword = NULL;
		expanded = get_nextarg(wrt, explst, arg);
		if (!get_explst_word(wrt, explst, explstword))
			return (0);
		status = expand_wildcard(&newargs, explstword, expanded);
		if (!status && !lst_addback_content(&newargs,
				unquote_arg(explstword, expanded)))
			return (ft_lstclear(&newargs, free), 0);
		else if (status == -1)
			return (ft_lstclear(&newargs, free), 0);
		free(expanded);
		ft_lstclear(&explstword, free);
	}
	return (free(arg), 1);
}
