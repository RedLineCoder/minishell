/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:41 by moztop            #+#    #+#             */
/*   Updated: 2024/10/09 13:28:53 by moztop           ###   ########.fr       */
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

char	*expander(char *arg)
{
	t_list	*explst;
	char	*expanded;

	expanded = expand_dollar(arg, &explst);
	if (!explst)
		return (NULL);
	return (expanded);
}
