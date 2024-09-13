/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:18:50 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/13 01:02:08 by emyildir         ###   ########.fr       */
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

t_unode	*get_unode(void *cmd)
{
	t_unode *const	node = ft_calloc(sizeof(t_unode), 1);

	if (!node)
		return NULL;
	node->type = UNODE;
	node->cmd = cmd;
	return (node);
}
