/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/27 18:21:59 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_logic(char *ps, char *pe, t_cmd **cmd)
{
	t_logiccmd *const	op = ft_calloc(sizeof(t_logiccmd), 1);
	t_part				ln;
	int					status;

	if (!op)
		return (1);
	ln = ft_divide(ps, pe, LOGIC_OP, 1);
	op->type = LOGIC;
	op->op_type = get_logicop(ln.lfte, ln.rghts);
	*cmd = (t_cmd *)op;
	if (!peek(ln.lfts, ln.lfte, TKN_NONE))
		return (syntax_panic(ln.lfte), 258);
	status = parser(ln.lfts, ln.lfte, &op->left);
	if (status)
		return (status);
	status = parser(ln.rghts, ln.rghte, &op->right);
	if (status)
		return (status);
	if (!peek(ln.rghts, ln.rghte, TKN_NONE))
		return (syntax_panic(ln.rghts), 1);
	return (status);
}

int	parse_pipe(char *ps, char *pe, t_cmd **cmd)
{
	t_pipecmd *const	pipe = ft_calloc(sizeof(t_pipecmd), 1);

	if (!pipe)
		return (1);
	pipe->type = PIPE;
	*cmd = (t_cmd *)pipe;
	return (init_pipes(ps, pe, &(pipe->pipelist)));
}

int	parse_block(char *ps, char *pe, t_cmd **cmd)
{
	t_blockcmd *const block = ft_calloc(sizeof(t_blockcmd), 1);
	int				status;
	char			*ts;
	char			*te;

	if (!block)
		return (1);
	block->type = SUBSHELL;
	*cmd = (t_cmd *)block;
	get_token(&ps, &pe, &ts, &te);
	te = pass_block(ts, pe);
	if (!*te)
		return (syntax_panic(te), 1);
	if (!peek(ts + 1, te, TKN_NONE))
		return (syntax_panic(ts + 1), 258);
	status = parser(ts + 1, te, &block->subshell);
	if (status)
		return (status);
	return (parse_redirs(te + 1, pe, 1, &(block->redirs)));
}

int	parse_exec(char *ps, char *pe, t_cmd **cmd)
{
	t_execcmd *const	exec = ft_calloc(sizeof(t_execcmd), 1);
	int					status;

	if (!exec)
		return (1);
	exec->type = EXEC;
	*cmd = (t_cmd *)exec;
	status = parse_redirs(ps, pe, 0, &(exec->redirs));
	if (status)
		return (status);
	status = parse_args(ps, pe, &(exec->args));
	if (status)
		return (status);
	return (0);
}

int	parser(char *ps, char *pe, t_cmd **cmd)
{
	int	status;

	if (!peek(ps, pe, TKN_NONE))
		return (0);
	if (peek(ps, pe, TKN_NONE) == BLK_CLS)
		return (258);
	if (peek(ps, pe, LOGIC_OP))
		status = parse_logic(ps, pe, cmd);
	else if (peek(ps, pe, PIPE_OP))
		status = parse_pipe(ps, pe, cmd);
	else if (peek(ps, pe, TKN_NONE) == BLK_OP)
		status = parse_block(ps, pe, cmd);
	else
		status = parse_exec(ps, pe, cmd);
	return (status);
}
