/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/14 20:45:17 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_node	*get_node(void *cmd, int nullcheck)
{
	t_node *const	node = ft_calloc(sizeof(t_node), 1);

	if (!node || (nullcheck && !cmd))
		return (NULL);
	node->cmd = (t_cmd *)cmd;
	return (node);
}

t_cmd	*parse_cmd(char **ps)
{
	char			*ts;
	char			*te;
	t_tokens		token;
	t_cmd			*(*funcs[7])(char **, char *, char *);

	ft_memset(funcs, 0, sizeof(t_cmd *) * 7);
	funcs[REDIR] = parse_redir;
	funcs[CMD_OP] = parse_cmdop;
	funcs[ARG] = parse_arg;
	funcs[BLOCK] = parse_block;
	token = get_token(ps, &ts, &te);
	if (funcs[token])
		return (funcs[token](ps, ts, te));
	return (NULL);
}

int	set_node(char *ps, t_node *node, t_tokens next)
{
	if (next == CMD_OP)
	{
		node->right = get_node(parse_cmd(&ps), 1);
		if (!node->right)
			return (0);
		if (!parser(ps, node->right))
			return (0);
	}
	else if (next)
	{
		node->left = get_node(parse_exec(&ps, NULL, NULL), 1);
		if (!node->left)
			return (0);
		if (!parser(ps, node))
			return (0);
	}
	return (1);
}

int	err_syntax(t_tokens next, t_node *node)
{
	(void)next;
	if (!node->left && node->right)
		return (printf("%s `%i'\n", ERR_TKN_SYNTAX, node->right->cmd->type), 258);
	return (0);
}

int	parser(char *ps, t_node *node)
{
	t_tokens	next;

	next = peek(ps);
	if (next == BLOCK)
	{
		node->left = get_node(parse_cmd(&ps), 1);
		if (!node->left)
			return (0);
		if (!parser(((t_blockcmd *)node->left->cmd)->line, node->left))
			return (0);
	}
	if (!set_node(ps, node, peek(ps)))
		return (0);
	if (err_syntax(next, node))
		return (0);
	return (1);
}
