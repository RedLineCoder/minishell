/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:41 by moztop            #+#    #+#             */
/*   Updated: 2024/11/16 19:01:07 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	set_exptrack(t_list **explst, int start, int end)
{
	t_list	*item;
	int		*locs;

	if (!explst)
		return (1);
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
			return (free(expanded), NULL);
		free(expanded);
		ft_lstclear(&explst, free);
		lst = lst->next;
	}
	return (newargs);
}
