/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:11:56 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/18 11:40:46 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	syntax_panic(char *ps)
{
	t_tokens	token;
	char		*pe;
	char		*ts;
	char		*te;

	pe = ps + ft_strlen(ps);
	token = get_token(&ps, &pe, &ts, &te);
	ft_putstr_fd("-msh: syntax error near unexpected token ", 2);
	ft_putstr_fd("'", 2);
	if (!token || token == ERR_QUOTE)
		write(2, "newline", 7);
	else
		write(2, ts, te - ts);
	ft_putendl_fd("'", 2);
	return (258);
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
		if (!lst_add_back_content(args, ts))
			return (free(ts), 1);
		return (parse_args(ps, pe, args));
	}
	else if (token == REDIR_OP)
	{
		get_token(&ps, &pe, &ts, &te);
		return (parse_args(ps, pe, args));
	}
	return (0);
}

int	parse_redir(char *ps, char *pe, t_redircmd **cmd)
{
	t_redircmd *const	redir = ft_calloc(sizeof(t_redircmd), 1);
	char				*fd;
	char				*ts;
	char				*te;

	*cmd = redir;
	get_token(&ps, &pe, &ts, &te);
	fd = ft_strndup(ts, te - ts);
	if (!fd || !redir)
		return (free(fd), 1);
	redir->type = REDIR;
	redir->redir_type = get_redir(ts, te);
	redir->fd = redir->redir_type % 2;
	if (ft_isdigit(*ts))
		redir->fd = ft_atoi(fd);
	if (peek(ps, pe, TKN_NONE) == ARG)
	{
		get_token(&ps, &pe, &ts, &te);
		if (!lst_add_back_content(&(redir->args), ft_strndup(ts, te - ts)))
			return (1);
	}
	else
		return (free(fd), syntax_panic(te));
	return (free(fd), 0);
}

int	parse_redirs(char *ps, char *pe, int block, t_list **redirs)
{
	t_tokens	token;
	t_redircmd	*cmd;
	char		*ts;
	int			status;

	token = get_token(&ps, &pe, &ts, NULL);
	if (token == REDIR_OP)
	{
		status = parse_redir(ts, pe, &cmd);
		if (status)
			return (status);
		if (!lst_add_back_content(redirs, cmd))
			return (free(cmd), 1);
		get_token(&ps, &pe, &ts, NULL);
	}
	else if (!token)
		return (0);
	else if (token != ARG || block)
		return (syntax_panic(ts));
	return (parse_redirs(ps, pe, block, redirs));
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
			return (syntax_panic(ln.lfte));
		status = parser(ln.lfts, ln.lfte, &cmd);
		if (status)
			return (status);
		if (!peek(ln.rghts, ln.rghte, TKN_NONE))
			return (syntax_panic(ln.rghts));
		if (!lst_add_back_content(pipelist, cmd))
			return (free(cmd), 1);
		status = init_pipes(ln.rghts, ln.rghte, pipelist);
	}
	else
	{
		status = parser(ps, pe, &cmd);
		if (!lst_add_back_content(pipelist, cmd))
			return (free(cmd), 1);
	}
	return (status);
}
