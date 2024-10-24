/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:08 by moztop            #+#    #+#             */
/*   Updated: 2024/10/24 21:00:13 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_part	ft_divide(char *s, char *e, t_tokens tkn, int rev)
{
	t_part	ln;
	char	*ts;
	char	*te;

	ln.lfts = s;
	while (s && s != e)
	{
		pass_block(s, &s, e);
		if (get_token(&s, &e, &ts, &te) == tkn)
		{
			ln.lfte = ts;
			ln.rghts = te;
			if (!rev)
				break ;
		}
	}
	ln.rghte = e;
	return (ln);
}

void	free_list(t_list *lst)
{
	while (lst)
	{
		free(lst);
		lst = lst->next;
	}
}

int	lst_addback_content(t_list **lst, void *content)
{
	t_list	*item;

	item = ft_lstnew(content);
	if (!item || !lst)
		return (0);
	ft_lstadd_back(lst, item);
	return (1);
}
