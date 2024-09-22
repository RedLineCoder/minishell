/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/22 15:20:52 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*parse_cmd(char **ps, char **pe)
{
	char		*ts;
	char		*te;
	t_tokens	token;

	t_cmd		*(*funcs[4])(char **, char **, char *, char *);
	ft_memset(funcs, 0, sizeof(t_cmd *) * 4);
	funcs[REDIR_OP] = parse_redir;
	funcs[ARG] = parse_arg;
	token = get_token(ps, pe, &ts, &te);
	if (funcs[token])
		return (funcs[token](ps, pe, ts, te));
	return (NULL);
}

t_cmd	*parse_logic(char *ps, char *pe)
{
	t_opcmd *const op = ft_calloc(sizeof(t_opcmd), 1);
	t_lnsplit		ln;

	if (!op)
		return (NULL);
	ln = ft_lnsplit(ps, pe, LOGIC_OP);
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

	if (!pipe)
		pipe = ft_calloc(sizeof(t_pipecmd), 1);
	if (!pipe)
		return (NULL);
	pipe->type = PIPE;
	if (peek(ps, pe, PIPE_OP))
	{
		ln = ft_lnsplit(ps, pe, PIPE_OP);
		cmd = parser(ln.rghts, ln.rghte);
		lst = ft_lstnew(cmd);
		ft_lstadd_front(&pipe->pipelist, lst);
		parse_pipe(ln.lfts, ln.lfte, pipe);
	}
	else
	{
		cmd = parser(ps, pe);
		lst = ft_lstnew(cmd);
		ft_lstadd_front(&pipe->pipelist, lst);
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
