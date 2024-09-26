/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:11:56 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/26 18:57:09 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	syntax_panic(char *ls, char *msg)
{
	char	*ps;
	char	*pe;
	char	*ts;
	char	*te;

	(void)ls;
	(void)pe;
	(void)ps;
	(void)ts;
	(void)te;
	(void)msg;
	//ps = ls;
	/* pe = ps + ft_strlen(ps);
	get_token(&ps, &pe, &ts, &te);
	if (msg)
	{
		ft_putstr_fd(msg, 2);
		ft_putstr_fd("'", 2);
		if (!peek(ts, te, TKN_NONE))
			write(2, "newline", 7);
		else
			write(2, ts, te - ts);
		ft_putendl_fd("'", 2);
	} */
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
	t_redircmd	*cmd;
	char		*ts;
	char		*te;

	if (peek(ps, pe, TKN_NONE) == REDIR_OP)
	{
		get_token(&ps, &pe, &ts, &te);
		if (parse_redir(ts, te, &cmd))
			return (1);
		if (peek(ps, pe, TKN_NONE) == ARG)
		{
			get_token(&ps, &pe, &ts, &te);
			cmd->s_spec = ts, cmd->e_spec = te;
		}
		else
			return (syntax_panic(te, ERR_TKN), 258);
		ft_lstadd_back(redirs, ft_lstnew(cmd));
	}
	else if (!peek(ps, pe, TKN_NONE))
		return (0);
	else if (peek(ps, pe, TKN_NONE) != ARG || block)
		return (syntax_panic(ps, ERR_TKN), 258);
	return(parse_redirs(ps, pe, block, redirs));
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
			return (syntax_panic(ln.lfte, ERR_TKN), 258);
		if (!peek(ln.rghts, ln.rghte, TKN_NONE))
			return (syntax_panic(ln.rghts, ERR_TKN), 1);
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
