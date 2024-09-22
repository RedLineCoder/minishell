/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/22 16:35:45 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*parse_logic(char *ps, char *pe)
{
	t_opcmd *const op = ft_calloc(sizeof(t_opcmd), 1);
	t_lnsplit		ln;

	if (!op)
		return (NULL);
	ln = ft_lnsplit(ps, pe, LOGIC_OP, 1);
	op->type = LOGIC;
	op->op_type = get_logicop(ln.lfte, ln.rghts);
	op->left = parser(ln.lfts, ln.lfte);
	op->right = parser(ln.rghts, ln.rghte);
	return ((t_cmd *)op);
}

t_cmd	*parse_pipe(char *ps, char *pe, t_pipecmd *pipe)
{
	t_lnsplit	ln;
	t_cmd		*cmd;
	t_list		*lst;

	(void)cmd, (void)lst;
	if (!pipe)
		pipe = ft_calloc(sizeof(t_pipecmd), 1);
	if (!pipe)
		return (NULL);
	pipe->type = PIPE;
	if (peek(ps, pe, PIPE_OP))
	{
		ln = ft_lnsplit(ps, pe, PIPE_OP, 0);
		cmd = parser(ln.lfts, ln.lfte);
		lst = ft_lstnew(cmd);
		ft_lstadd_back(&pipe->pipelist, lst);
		parse_pipe(ln.rghts, ln.rghte, pipe);
	}
	else
	{
		cmd = parser(ps, pe);
		lst = ft_lstnew(cmd);
		ft_lstadd_back(&pipe->pipelist, lst);
	}
	return ((t_cmd *)pipe);
}

t_cmd	*parse_block(char *ps, char *pe)
{
	t_blockcmd *const block = ft_calloc(sizeof(t_blockcmd), 1);
	char			*ts;
	char			*te;

	if (!block)
		return (NULL);
	block->type = SUBSHELL;
	get_token(&ps, &pe, &ts, &te);
	te = pass_block(ts, pe);
	block->subshell = parser(ts + 1, te);
	return ((t_cmd *)block);
}

t_cmd	*parser(char *ps, char *pe)
{
	t_cmd		*cmd;

	if (peek(ps, pe, LOGIC_OP))
		cmd = parse_logic(ps, pe);
	else if (peek(ps, pe, PIPE_OP))
		cmd = parse_pipe(ps, pe, NULL);
	else if (peek(ps, pe, TKN_NONE) == BLK_OP)
		cmd = parse_block(ps, pe);
	else
		cmd = parse_exec(&ps, &pe, NULL, NULL);
	if (!cmd)
		return (NULL);
	return (cmd);
}
