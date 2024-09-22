/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/23 02:27:48 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*parse_logic(char *ps, char *pe, t_msh *msh)
{
	t_logiccmd *const	op = ft_calloc(sizeof(t_logiccmd), 1);
	t_part				ln;

	if (!op)
		return (NULL);
	ln = ft_divide(ps, pe, LOGIC_OP, 1);
	op->type = LOGIC;
	op->op_type = get_logicop(ln.lfte, ln.rghts);
	op->left = parser(ln.lfts, ln.lfte, msh);
	if (!op->left)
		return (syntax_panic(ln.lfte, pe, 258, msh));
	op->right = parser(ln.rghts, ln.rghte, msh);
	if (!op->right)
		return (syntax_panic(ln.rghts, pe, 1, msh));
	return ((t_cmd *)op);
}

int	add_pipe(char *ps, char *pe, t_msh *msh, t_pipecmd *pipe)
{
	t_part		ln;
	t_cmd		*cmd;
	t_list		*lst;

	if (peek(ps, pe, PIPE_OP))
	{
		ln = ft_divide(ps, pe, PIPE_OP, 0);
		cmd = parser(ln.lfts, ln.lfte, msh);
		if (!cmd)
			return(syntax_panic(ln.lfte, pe, 258, msh), 0);
		lst = ft_lstnew(cmd);
		ft_lstadd_back(&pipe->pipelist, lst);
		parse_pipe(ln.rghts, ln.rghte, msh, pipe);
	}
	else
	{
		cmd = parser(ps, pe, msh);
		if (!cmd)
			return(syntax_panic(ps, pe, 1, msh), 0);
		lst = ft_lstnew(cmd);
		ft_lstadd_back(&pipe->pipelist, lst);
	}
	return (1);
}

t_cmd	*parse_pipe(char *ps, char *pe, t_msh *msh, t_pipecmd *pipe)
{
	if (!pipe)
		pipe = ft_calloc(sizeof(t_pipecmd), 1);
	if (!pipe)
		return (NULL);
	pipe->type = PIPE;
	if (!add_pipe(ps, pe, msh, pipe))
		return (NULL);
	return ((t_cmd *)pipe);
}

t_cmd	*parse_block(char *ps, char *pe, t_msh *msh)
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
	block->subshell = parser(ts + 1, te, msh);
	ps = te;
	while (ps != pe)
	{
		if (get_token(&ps, &pe, &ts, &te) == REDIR_OP)
		{
			cmd = parse_redir(&ps, &pe, msh);
			item = ft_lstnew(cmd);
			if (!cmd || !item)
				return (NULL);
			ft_lstadd_back(&block->redirs, item);
		}
	}
	return ((t_cmd *)block);
}

void	*syntax_panic(char *ps, char *pe, int status, t_msh *msh)
{
	char	*ts;
	char	*te;

	if (status == 258)
		msh->status = 258;
	if (status == 1)
		msh->status = 1;
	ft_putstr_fd(ERR_TKN, 2);
	ft_putstr_fd("'", 2);
	if (!get_token(&ps, &pe, &ts, &te))
		write(2, "newline", 7);
	else
		write(2, ts, te - ts);
	ft_putendl_fd("'", 2);
	return (NULL);
}

t_cmd	*parser(char *ps, char *pe, t_msh *msh)
{
	t_cmd		*cmd;

	if (!peek(ps, pe, TKN_NONE))
		return (NULL);
	if (peek(ps, pe, TKN_NONE) == BLK_CLS)
		return (syntax_panic(ps, pe, 258, msh));
	if (peek(ps, pe, LOGIC_OP))
		cmd = parse_logic(ps, pe, msh);
	else if (peek(ps, pe, PIPE_OP))
		cmd = parse_pipe(ps, pe, msh, NULL);
	else if (peek(ps, pe, TKN_NONE) == BLK_OP)
		cmd = parse_block(ps, pe, msh);
	else
		cmd = parse_exec(ps, pe, msh);
	if (!cmd)
		return (NULL);
	return (cmd);
}
