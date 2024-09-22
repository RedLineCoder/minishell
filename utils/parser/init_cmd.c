/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:11:56 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/20 19:04:29 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_arg(char **ps, char **pe, char *ts, char *te)
{
	t_argcmd *const	arg = ft_calloc(sizeof(t_argcmd), 1);

	if (!arg)
		return (NULL);
	(void)ps;
	(void)pe;
	arg->type = ARG;
	arg->s_arg = ts;
	arg->e_arg = te;
	return ((t_cmd *) arg);
}

/* t_cmd	*parse_block(char **ps, char **pe, char *ts, char *te)
{
	t_blockcmd	*const	block = ft_calloc(sizeof(t_blockcmd), 1);

	(void)ps;
	(void)pe;
	if (!block)
		return (NULL);
	block->type = SUBSHELL;
	return ((t_cmd *)block);
} */

t_cmd	*parse_exec(char **ps, char **pe, char *ts, char *te)
{
	t_execcmd *const	exec = ft_calloc(sizeof(t_execcmd), 1);
	t_cmd				*cmd;
	t_list				*item;
	t_tokens			token;

	((void)ts, (void)te);
	if (!exec)
		return (NULL);
	token = peek(*ps, *pe, NULL);
	if (!(token == ARG || token == REDIR_OP))
		return (free(exec), NULL);
	exec->type = CMD;
	while (token == ARG || token == REDIR_OP)
	{
		cmd = parse_cmd(ps, pe);
		item = ft_lstnew(cmd);
		if (!cmd || !item)
			return (free(exec), free(cmd), free(item), NULL);
		if (cmd->type == ARG)
			ft_lstadd_back(&exec->args, item);
		else if (cmd->type == REDIR_OP)
			ft_lstadd_back(&exec->redirs, item);
		token = peek(*ps, *pe, NULL);
	}
	return ((t_cmd *)exec);
}

/* t_cmd	*parse_cmdop(char **ps, char **pe, char *ts, char *te)
{
	t_opcmd *const	op = ft_calloc(sizeof(t_opcmd), 1);

	if (!op)
		return (NULL);
	(void)ps;
	(void)pe;
	op->type = LOGIC;
	if (ft_strncmp(ts, "|", te - ts))
		op->type = PIPE;
	op->op_type = get_cmdop(ts, te);
	return ((t_cmd *)op);
} */

t_cmd	*parse_redir(char **ps, char **pe, char *ts, char *te)
{
	t_redircmd *const	redir = ft_calloc(sizeof(t_redircmd), 1);
	char				*fd;

	if (!redir)
		return (NULL);
	redir->type = REDIR_OP;
	fd = ft_strndup(ts, te - ts);
	if (!fd)
		return (free(redir), NULL);
	while (!ft_strchr("<>", *ts))
		ts++;
	redir->redir_type = get_redir(ts, te);
	redir->fd = redir->redir_type != 1 && redir->redir_type != 4;
	if (*fd != *ts)
		redir->fd = ft_atoi(fd);
	free(fd);
	if (peek(*ps, *pe, NULL) == ARG)
		get_token(ps, pe, &redir->s_spec, &redir->e_spec);
	return ((t_cmd *) redir);
}
