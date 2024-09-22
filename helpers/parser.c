/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/23 00:32:42 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*parse_logic(char *ps, char *pe)
{
	t_logiccmd *const	op = ft_calloc(sizeof(t_logiccmd), 1);
	t_part				ln;

	if (!op)
		return (NULL);
	ln = ft_divide(ps, pe, LOGIC_OP, 1);
	op->type = LOGIC;
	op->op_type = get_logicop(ln.lfte, ln.rghts);
	op->left = parser(ln.lfts, ln.lfte);
	op->right = parser(ln.rghts, ln.rghte);
	return ((t_cmd *)op);
}

t_cmd	*parse_pipe(char *ps, char *pe, t_pipecmd *pipe)
{
	t_part		ln;
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
		ln = ft_divide(ps, pe, PIPE_OP, 0);
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
	t_cmd			*cmd;
	t_list			*item;
	char			*ts;
	char			*te;

	if (!block)
		return (NULL);
	block->type = SUBSHELL;
	get_token(&ps, &pe, &ts, &te);
	te = pass_block(ts, pe);
	block->subshell = parser(ts + 1, te);
	ps = te;
	while (ps != pe)
	{
		if (get_token(&ps, &pe, &ts, &te) == REDIR_OP)
		{
			cmd = parse_redir(&ps, &pe, ts, te);
			item = ft_lstnew(cmd);
			if (!cmd || !item)
				return (NULL);
			ft_lstadd_back(&block->redirs, item);
		}
	}
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
		cmd = parse_exec(ps, pe);
	if (!cmd)
		return (NULL);
	return (cmd);
}
