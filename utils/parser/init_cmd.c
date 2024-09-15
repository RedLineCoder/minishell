/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:11:56 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/15 06:57:03 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_arg(char **ps, char *ts, char *te)
{
	t_argcmd *const	arg = ft_calloc(sizeof(t_argcmd), 1);

	if (!arg)
		return (NULL);
	(void)ps;
	arg->type = ARG;
	arg->s_arg = ts;
	arg->e_arg = te;
	return ((t_cmd *) arg);
}

t_cmd	*parse_block(char **ps, char *ts, char *te)
{
	t_blockcmd	*const	block = ft_calloc(sizeof(t_blockcmd), 1);
	char		*line;

	(void)ps;
	line = ft_substr(ts, 1, (te - ts) - 2);
	if (!line)
		return (NULL);
	if (!block)
		return (free(line), NULL);
	block->type = BLOCK;
	block->line = line;
	return ((t_cmd *)block);
}

t_cmd	*parse_exec(char **ps, char *ts, char *te)
{
	t_execcmd *const	exec = ft_calloc(sizeof(t_execcmd), 1);
	t_cmd				*cmd;
	t_list				*item;
	t_tokens			token;

	((void)ts, (void)te);
	if (!exec)
		return (NULL);
	token = peek(*ps);
	if (!(token == ARG || token == REDIR))
		return (free(exec), NULL);
	exec->type = EXEC;
	while (token == ARG || token == REDIR)
	{
		cmd = parse_cmd(ps);
		item = ft_lstnew(cmd);
		if (!cmd || !item)
			return (free(exec), free(cmd), free(item), NULL);
		if (cmd->type == ARG)
			ft_lstadd_back(&exec->args, item);
		else if (cmd->type == REDIR)
			ft_lstadd_back(&exec->redirs, item);
		token = peek(*ps);
	}
	return ((t_cmd *)exec);
}

t_cmd	*parse_cmdop(char **ps, char *ts, char *te)
{
	t_opcmd *const	op = ft_calloc(sizeof(t_opcmd), 1);

	if (!op)
		return (NULL);
	(void)ps;
	op->type = CMD_OP;
	op->op_type = get_cmdop(ts, te);
	return ((t_cmd *)op);
}

t_cmd	*parse_redir(char **ps, char *ts, char *te)
{
	t_redircmd *const	redir = ft_calloc(sizeof(t_redircmd), 1);
	char				*fd;

	if (!redir)
		return (NULL);
	redir->type = REDIR;
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
	if (peek(*ps) == ARG)
		get_token(ps, &redir->s_spec, &redir->e_spec);
	return ((t_cmd *) redir);
}