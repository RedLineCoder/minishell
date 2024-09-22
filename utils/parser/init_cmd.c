/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:11:56 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/23 00:18:09 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	parse_args(char *ps, char *pe, t_list **args)
{
	t_list		*arg;
	char		*word;
	char		*ts;
	char		*te;

	while (ps != pe)
	{
		if (get_token(&ps, &pe, &ts, &te) == ARG)
		{
			word = ft_strndup(ts, te - ts);
			arg = ft_lstnew(word);
			if (!arg || !word)
				return (free(arg), free(word), 0);
			ft_lstadd_back(args, arg);
		}
		else
			get_token(&ps, &pe, &ts, &te);
	}
	return (1);
}

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
	redir->fd = redir->redir_type % 2;
	if (*fd != *ts)
		redir->fd = ft_atoi(fd);
	free(fd);
	if (peek(*ps, *pe, TKN_NONE) == ARG)
		get_token(ps, pe, &redir->s_spec, &redir->e_spec);
	else
		return (NULL);
	return ((t_cmd *) redir);
}

t_cmd	*parse_exec(char *ps, char *pe)
{
	t_execcmd *const	exec = ft_calloc(sizeof(t_execcmd), 1);
	t_cmd				*cmd;
	t_list				*item;
	char				*ts;
	char				*te;

	if (!exec)
		return (NULL);
	exec->type = EXEC;
	if (!parse_args(ps, pe, &(exec->args)))
		return (NULL);
	while (ps != pe)
	{
		if (get_token(&ps, &pe, &ts, &te) == REDIR_OP)
		{
			cmd = parse_redir(&ps, &pe, ts, te);
			item = ft_lstnew(cmd);
			if (!cmd || !item)
				return (NULL);
			ft_lstadd_back(&exec->redirs, item);
		}
	}
	return ((t_cmd *)exec);
}

/* int	parse_args(char *ps, char *pe, t_list *lst)
{
	t_argcmd	*arg;
	t_list		*item;
	char		*ts;
	char		*te;

	while (ps != pe)
	{
		if (get_token(&ps, &pe, &ts, &te) == ARG)
		{
			arg = ft_calloc(sizeof(t_argcmd), 1);
			if (!arg)
				return (1);
			arg->type = ARG;
			arg->s_arg = ts;
			arg->e_arg = te;
			item = ft_lstnew(arg);
			if (!item)
				return (free(arg), 1);
		}
		else
			get_token(&ps, &pe, &ts, &te);
	}
	return (0);
} */
