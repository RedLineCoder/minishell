/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:41 by moztop            #+#    #+#             */
/*   Updated: 2024/10/25 19:29:29 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	set_exptrack(t_list **explst, int start, int end)
{
	t_list	*item;
	int		*locs;

	locs = ft_calloc(sizeof(int), 2);
	if (!locs)
		return (0);
	*locs = start;
	*(locs + 1) = end - 1;
	item = ft_lstnew(locs);
	if (!item)
		return (free(locs), 0);
	ft_lstadd_back(explst, item);
	return (1);
}

int	is_expanded(t_list *explst, int index)
{
	while (explst)
	{
		if (index >= (((int *)explst->content)[0])
			&& index <= (((int *)explst->content)[1]))
			return (1);
		explst = explst->next;
	}
	return (0);
}

int	unquoted_size(t_list *explst, char *arg)
{
	t_write *const	wrt = &(t_write){0};

	while (arg[wrt->a_i])
	{
		if (arg[wrt->a_i] == '\'' && !wrt->qd && !is_expanded(explst, wrt->a_i))
			wrt->qs = !wrt->qs;
		if (arg[wrt->a_i] == '"' && !wrt->qs && !is_expanded(explst, wrt->a_i))
			wrt->qd = !wrt->qd;
		if (((!wrt->qd && arg[wrt->a_i] == '\'') || (!wrt->qs
					&& arg[wrt->a_i] == '"')) && !is_expanded(explst, wrt->a_i))
		{
			wrt->a_i++;
			continue ;
		}
		wrt->e_i++;
		wrt->a_i++;
	}
	return (wrt->e_i);
}

char	*unquote_arg(t_list *explst, char *arg)
{
	t_write *const	wrt = &(t_write){0};
	char			*exp;

	exp = ft_calloc(sizeof(char), unquoted_size(explst, arg) + 1);
	if (!exp || !arg)
		return (free(exp), free(arg), NULL);
	while (arg[wrt->a_i])
	{
		if (arg[wrt->a_i] == '\'' && !wrt->qd && !is_expanded(explst, wrt->a_i))
			wrt->qs = !wrt->qs;
		if (arg[wrt->a_i] == '"' && !wrt->qs && !is_expanded(explst, wrt->a_i))
			wrt->qd = !wrt->qd;
		if (((!wrt->qd && arg[wrt->a_i] == '\'') || (!wrt->qs
					&& arg[wrt->a_i] == '"')) && !is_expanded(explst, wrt->a_i))
		{
			wrt->a_i++;
			continue ;
		}
		exp[wrt->e_i++] = arg[wrt->a_i++];
	}
	return (free(arg), exp);
}

t_list	*expander(t_list *args, t_msh *msh)
{
	t_list	*lst;
	t_list	*explst;
	t_list	*newargs;
	char	*expanded;

	newargs = NULL;
	lst = args;
	while (lst)
	{
		explst = NULL;
		expanded = expand_dollar(lst->content, &explst, msh);
		if (!split_words(&newargs, explst, expanded))
			return (NULL);
		ft_lstclear(&explst, free);
		lst = lst->next;
	}
	return (ft_lstclear(&args, free), newargs);
}
