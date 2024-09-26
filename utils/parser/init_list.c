/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:11:56 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/26 16:15:40 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	syntax_panic(char *ts, char *te, char *msg)
{
	if (msg)
	{
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("'", 2);
		if (!peek(ts, te, TKN_NONE))
			write(2, "newline", 7);
		else
			write(2, ts, te - ts);
		ft_putendl_fd("'", 2);
	}
}

int	parse_args(char *ps, char *pe, t_list **args)
{
	t_tokens	token;
	char		*ts;
	char		*te;

	token = get_token(&ps, &pe, &ts, &te);
	if (token == ARG)
	{
		ts = ft_strndup(ts, te - ts);
		if (!ts)
			return (1);
		ft_lstadd_back(args, ft_lstnew(ts));
		return (parse_args(ps, pe, args));
	}
	else if (token == REDIR_OP)
	{
		get_token(&ps, &pe, &ts, &te);
		return (parse_args(ps, pe, args));
	}
	return (0);
}

int	parse_redir(char *ts, char *te, t_redircmd **cmd)
{
	t_redircmd *const	redir = ft_calloc(sizeof(t_redircmd), 1);
	char				*fd;

	if (!redir)
		return (1);
	redir->type = REDIR;
	fd = ft_strndup(ts, te - ts);
	if (!fd)
		return (free(redir), 1);
	while (!ft_strchr("<>", *ts))
		ts++;
	redir->redir_type = get_redir(ts, te);
	redir->fd = redir->redir_type % 2;
	if (*fd != *ts)
		redir->fd = ft_atoi(fd);
	free(fd);
	*cmd = redir;
	return (0);
}

int	parse_redirs(char *ps, char *pe, int block, t_list **redirs)
{
	t_tokens	token;
	t_redircmd	*cmd;
	char		*ts;
	char		*te;

	token = get_token(&ps, &pe, &ts, &te);
	if (token == REDIR_OP)
	{
		if (parse_redir(ts, te, &cmd))
			return (1);
		token = get_token(&ps, &pe, &ts, &te);
		if (token == ARG)
			cmd->s_spec = ts, cmd->e_spec = te;
		else if (!token)
			return (syntax_panic(ps, pe, ERR_TKN), 1);
		else
			return (syntax_panic(ts, te, ERR_TKN), 258);
		ft_lstadd_back(redirs, ft_lstnew(cmd));
		return (parse_redirs(ps, pe, block,redirs));
	}
	else if ((token && token != ARG) || (token && block))
		return (syntax_panic(ts, te, ERR_TKN), 258);
	else if (token == ARG)
		parse_redirs(ps, pe, block,redirs);
	return(0);
}

int	init_pipes(char *ps, char *pe, t_list **pipelist)
{
	t_part		ln;
	t_cmd		*cmd;
	int			status;

	if (peek(ps, pe, PIPE_OP))
	{
		ln = ft_divide(ps, pe, PIPE_OP, 0);
		if (!peek(ln.lfts, ln.lfte, TKN_NONE))
			return (syntax_panic(ln.lfte, ln.rghts, ERR_TKN), 258);
		if (!peek(ln.rghts, ln.rghte, TKN_NONE))
			return (syntax_panic(ln.rghts, ln.rghte, ERR_TKN), 1);
		status = parser(ln.lfts, ln.lfte, &cmd);
		if (status)
			return (status);
		ft_lstadd_back(pipelist, ft_lstnew(cmd));
		status = init_pipes(ln.rghts, ln.rghte, pipelist);
		if (status)
			return (status);
	}
	else
	{
		status = parser(ps, pe, &cmd);
		if (status)
			return (status);
		ft_lstadd_back(pipelist, ft_lstnew(cmd));
	}
	return (0);
}
