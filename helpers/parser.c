/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 19:41:13 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_binode	*parser(char *prompt, int type)
{
	char		*ps;
	t_tokens	token;
	t_binode	*const root = get_binode(NULL, NULL);	
	
	if (!root)
		return (NULL);
	ps = prompt;
	root->cmd = ft_calloc(sizeof(t_blockcmd), 1);
	root->cmd->type = type;
	t_cmd *cmd = parse_exec(&ps, NULL, NULL);
	root->left = (t_node *) get_unode(cmd);
	((t_unode *)root->left)->cmd = cmd;
	if (peek_next(ps) == COND || peek_next(ps) == PIPE)
	{
		token = get_token(&ps, NULL, NULL);
		root->right = (t_node *) parser(ps, token);
	}
	return (root);
}
