/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:41 by moztop            #+#    #+#             */
/*   Updated: 2024/10/11 18:46:52 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	set_exptrack(t_list **explst, char *start, char *end)
{
	t_list	*item;
	char	**ptrs;

	ptrs = ft_calloc(sizeof(char *), 2);
	if (!ptrs)
		return (0);
	*ptrs = start;
	*(ptrs + 1) = end;
	item = ft_lstnew(ptrs);
	if (!item)
		return (free(ptrs), 0);
	ft_lstadd_back(explst, item);
	return (1);
}

int	is_expanded(t_list *explst, char *ptr)
{
	while (explst)
	{
		if (ptr >= (*(char **)explst->content)
			&& ptr <= (*(char **)explst->content + 1))
			return (1);
		explst = explst->next;
	}
	return (0);
}

t_list	*expander(t_list *args, t_msh *msh)
{
	t_list	*explst;
	t_list	*newargs;
	char	*expanded;
	int		status;

	newargs = NULL;
	status = 0;
	while (args)
	{
		explst = NULL;
		expanded = expand_dollar(args->content, ft_itoa(msh->last_status), &explst);
		status = expand_wildcard(&newargs, explst, expanded);
		if (!status && !lst_addback_content(&newargs, unquote_arg(explst, expanded)))
			return (ft_lstclear(&newargs, free), NULL);
		else if (status == -1)
			return (ft_lstclear(&newargs, free), NULL);
		ft_lstclear(&explst, free);
		args = args->next;
	}
	return (newargs);
}