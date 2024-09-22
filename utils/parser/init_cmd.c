/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:11:56 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/23 02:45:31 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	parse_args(char *ps, char *pe, t_list **args, t_msh *msh)
{
	t_list		*arg;
	t_tokens	token;
	char		*word;
	char		*ts;
	char		*te;

	while (ps != pe)
	{
		token = get_token(&ps, &pe, &ts, &te);
		if (token == ARG)
		{
			word = ft_strndup(ts, te - ts);
			arg = ft_lstnew(word);
			if (!arg || !word)
				return (free(arg), free(word), 0);
			ft_lstadd_back(args, arg);
		}
		if (token == BLK_OP && peek(ps, pe, TKN_NONE) == BLK_CLS)
			return (get_token(&ps, &pe, &ts, &te), (int)syntax_panic(ps, pe, 258, msh));
		else if (token == BLK_OP || peek(ps, pe, TKN_NONE) == BLK_CLS)
			return ((int)syntax_panic(ps, pe, 258, msh));
		if (token == REDIR_OP && peek(ps, pe, TKN_NONE) == ARG)
			get_token(&ps, &pe, &ts, &te);
	}
	return (1);
}

t_cmd	*parse_redir(char **ps, char **pe, t_msh *msh)
{
	t_redircmd *const	redir = ft_calloc(sizeof(t_redircmd), 1);
	char				*fd;
	char				*ts;
	char				*te;

	if (!redir)
		return (NULL);
	redir->type = REDIR_OP;
	get_token(ps, pe, &ts, &te);
	fd = ft_strndup(ts, te - ts);
	if (!fd)
		return (free(redir), NULL);
	while (!ft_strchr("<>", *ts))
		ts++;
	redir->redir_type = get_redir(ts, te);
	redir->fd = redir->redir_type % 2;
	if (*fd != *ts)
		redir->fd = ft_atoi(fd);
	free(fd);
	if (peek(*ps, *pe, TKN_NONE) == ARG)
		get_token(ps, pe, &redir->s_spec, &redir->e_spec);
	else
		return (syntax_panic(*ps, *pe, 258, msh));
	return ((t_cmd *) redir);
}

t_cmd	*parse_exec(char *ps, char *pe, t_msh *msh)
{
	t_execcmd *const	exec = ft_calloc(sizeof(t_execcmd), 1);
	t_cmd				*cmd;
	t_list				*item;

	if (!exec)
		return (NULL);
	exec->type = EXEC;
	if (!parse_args(ps, pe, &(exec->args), msh))
		return (NULL);
	while (ps != pe)
	{
		if (peek(ps, pe, TKN_NONE) == REDIR_OP)
		{
			cmd = parse_redir(&ps, &pe, msh);
			item = ft_lstnew(cmd);
			if (!cmd || !item)
				return (NULL);
			ft_lstadd_back(&exec->redirs, item);
		}
		else
			get_token(&ps, &pe, NULL, NULL);
	}
	return ((t_cmd *)exec);
}
