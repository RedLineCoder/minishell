/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:08 by moztop            #+#    #+#             */
/*   Updated: 2024/10/29 10:07:05 by moztop           ###   ########.fr       */
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
	t_list	*temp;

	while (lst)
	{
		temp = lst;
		lst = lst->next;
		free(temp);
	}
}

int	lst_addback_content(t_list **lst, void *content)
{
	t_list	*item;

	item = ft_lstnew(content);
	if (!item || !lst || !content)
		return (free(item), free(content), 0);
	ft_lstadd_back(lst, item);
	return (1);
}
