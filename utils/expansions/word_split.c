/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:46:35 by moztop            #+#    #+#             */
/*   Updated: 2024/10/26 18:09:16 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_explst_word(t_write *wrt, t_list *explst, t_list **explstword, char *arg)
{
	int	start;
	int	index;

	index = 0;
	while (arg[index])
	{
		if (is_expanded(explst, wrt->e_i))
		{
			start = index;
			while (is_expanded(explst, wrt->e_i))
			{
				wrt->e_i++;
				index++;
			}
			if (!set_exptrack(explstword, start, index))
				return (ft_lstclear(explstword, free), 0);
		}
		else
		{
			wrt->e_i++;
			index++;
		}
	}
	return (1);
}

char	*get_nextarg(t_write *wrt, t_list *explst, char *arg)
{
	char	*nextarg;

	while (arg[wrt->a_i] && str_include(SPACES, arg[wrt->a_i]))
		wrt->a_i++;
	wrt->e_i = wrt->a_i;
	while (arg[wrt->a_i])
	{
		track_quotes(wrt, explst, arg);
		if (str_include(SPACES, arg[wrt->a_i]) && !wrt->qc)
			break ;
		wrt->a_i++;
	}
	nextarg = ft_strndup(&arg[wrt->e_i], wrt->a_i - wrt->e_i);
	return (nextarg);
}

int	split_words(t_list **newargs, t_list *explst, char *arg)
{
	t_write *const	wrt = &(t_write){0};
	t_list	*explstword;
	char	*expanded;
	int		status;

	while (arg[wrt->a_i])
	{
		explstword = NULL;
		expanded = get_nextarg(wrt, explst, arg);
		if (!get_explst_word(wrt, explst, &explstword, expanded))
			return (0);
		status = expand_wildcard(newargs, explstword, expanded);
		if (!status && !lst_addback_content(newargs,
				unquote_arg(explstword, expanded)))
			return (ft_lstclear(newargs, free), 0);
		else if (status == -1)
			return (ft_lstclear(newargs, free), 0);
		ft_lstclear(&explstword, free);
	}
	return (1);
}
