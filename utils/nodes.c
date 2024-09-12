/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:18:50 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 12:55:39 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_binode	*get_binode(void *left, void *right)
{
	t_binode *const	node = ft_calloc(sizeof(t_binode), 1);

	if (!node)
		return NULL;
	node->type = BINODE;
	node->left = left;
	node->right = right;
	return (node);
}

t_unode	*get_unode(void *next)
{
	t_unode *const	node = ft_calloc(sizeof(t_unode), 1);

	if (!node)
		return NULL;
	node->type = UNODE;
	node->next = next;
	return (node);
}
