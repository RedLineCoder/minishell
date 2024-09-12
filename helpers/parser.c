/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 23:02:40 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_binode	*parser(char *ps, int is_root)
{
	
	t_tokens	next;
	t_cmd		*cmd;
	t_binode	*const root = get_binode(NULL, NULL);	

	if (!root)
		return (NULL);
	if (!is_root)
	{
		root->cmd = parse_cmd(&ps);
		if (!root->cmd)
			return (free(root), NULL);
	}
	root->left = (t_node *) get_unode(NULL);
	if (!root->left)
		return (free(root), NULL);
	cmd = parse_exec(&ps, NULL, NULL);
	if (cmd)
		((t_unode *)root->left)->cmd = cmd;	
	next = peek(ps);
	if (next == CMD_OP)
		root->right = (t_node *) parser(ps, 0);
	return (root);
	
}
