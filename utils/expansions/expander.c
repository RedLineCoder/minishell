/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:54:41 by moztop            #+#    #+#             */
/*   Updated: 2024/10/10 19:58:31 by moztop           ###   ########.fr       */
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

int	unquoted_size(t_list *explst, char *arg)
{
	int	len;
	int	qd;
	int	qs;

	len = 0;
	qd = 0;
	qs = 0;
	while (*arg)
	{
		if (*arg == '\'' && !qd && !is_expanded(explst, *arg))
		{
			arg++;
			qs = !qs;
			continue ;
		}
		if (*arg == '"' && !qs && !is_expanded(explst, *arg))
		{
			arg++;
			qd = !qd;
			continue ;
		}
		arg++;
		len++;
	}
	return (len);
}

char	*unquote_arg(t_list *explst, char *arg)
{
	char	*exp;
	int		qd;
	int		qs;

	qd = 0;
	qs = 0;
	exp = ft_calloc(sizeof(char), unquoted_size(explst, arg) + 1);
	if (!exp)
		return (NULL);
	while (*arg)
	{
		if ((*arg == '\'' && !qd && !is_expanded(explst, *arg)) || (*arg == '"'
				&& !qs && !is_expanded(explst, *arg)))
		{
			arg++;
			if (*arg == '\'')
				qs = !qs;
			else
				qd = !qd;
			continue ;
		}
		*exp++ = *arg++;
	}
	return (free(arg), exp);
}

t_list	*expander(t_list *args, t_msh *msh)
{
	t_list	*explst;
	t_list	*newargs;
	char	*expanded;
	int		status;

	status = 0;
	while (args)
	{
		expanded = expand_dollar(args->content, &explst,
				ft_itoa(msh->last_status));
		if (!expanded)
			return (NULL);
		status = expand_wildcard(&newargs, explst, expanded);
		if (status == 0 && !lst_addback_content(&newargs, unquote_arg(explst,
					expanded)))
			return (ft_lstclear(&newargs, free), NULL);
		else if (status == -1)
			return (ft_lstclear(&newargs, free), NULL);
		ft_lstclear(&explst, free);
		explst = &(t_list){0};
		args = args->next;
	}
	return (ft_lstclear(&args, free), newargs);
}
