/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/13 03:40:43 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_binode	*parser(char *ps, char *pe, int is_block)
{
	t_tokens	next;
	t_cmd		*cmd;
	t_binode	*const root = get_binode(NULL, NULL);

	if (!root)
		return (NULL);
	if (!is_block)
	{
		root->cmd = parse_cmd(&ps, &pe);
		if (!root->cmd)
			return (free(root), NULL);
	}
	root->left = (t_node *) get_unode(NULL);
	if (!root->left)
		return (free(root), NULL);
	if (peek(ps, pe) == BLOCK)
		cmd = (t_cmd *)parse_block(&ps, &pe, NULL, NULL);
	else
		cmd = parse_exec(&ps, &pe, NULL, NULL);
	if (cmd)
		((t_unode *)root->left)->cmd = cmd;
	next = peek(ps, pe);
	if (next == CMD_OP)
		root->right = (t_node *) parser(ps, NULL, 0);
	return (root);
}

/* (void)is_block;
	char *ts;
	char *te;

	t_tokens token;
	token = get_token(&ps, &pe, &ts, &te);
	for (int i = 0; ts + i < te; i++)
		printf("%c", ts[i]);
	printf("\n");
	printf("Token: %i\n", token);
	return (NULL); */