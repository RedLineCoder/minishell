/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 18:43:36 by moztop            #+#    #+#             */
/*   Updated: 2023/12/25 16:42:21 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*temp;
	void	*content;

	if (!lst || !f || !del)
		return (NULL);
	new = NULL;
	while (lst != NULL)
	{
		content = (*f)(lst->content);
		if (!content)
			return (ft_lstclear(&new, del), NULL);
		temp = ft_lstnew(content);
		if (!temp)
			return (ft_lstclear(&new, del), del(content), NULL);
		ft_lstadd_back(&new, temp);
		lst = lst->next;
	}
	return (new);
}
