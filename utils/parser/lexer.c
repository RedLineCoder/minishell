/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:19:57 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/15 14:17:00 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_redir	get_redir(char *ts, char *te)
{
	int	len;

	while (ts < te && !ft_strchr(REDIRS, *ts))
		ts++;
	len = te - ts;
	if (!len)
		return (REDIR_NONE);
	if (!ft_strncmp(ts, ">", len))
		return (REDIR_OUTPUT);
	if (!ft_strncmp(ts, "<", len))
		return (REDIR_INPUT);
	if (!ft_strncmp(ts, ">>", len))
		return (REDIR_APPEND);
	if (!ft_strncmp(ts, "<<", len))
		return (REDIR_HDOC);
	return (REDIR_NONE);
}

t_cmdop	get_cmdop(char *ts, char *te)
{
	size_t	len;

	len = te - ts;
	if (!len)
		return (OP_NONE);
	if (!ft_strncmp(ts, "|", len))
		return (OP_PIPE);
	if (!ft_strncmp(ts, "||", len))
		return (OP_OR);
	if (!ft_strncmp(ts, "&", len))
		return (OP_ASYNC);
	if (!ft_strncmp(ts, "&&", len))
		return (OP_AND);
	return (OP_NONE);
}

t_tokens	get_token_type(char *ts, char *te)
{
	if (*ts == '(')
		return (BLK_OP);
	else if (*ts == ')')
		return (BLK_CLS);
	else if (get_redir(ts, te))
		return (REDIR);
	else if (get_cmdop(ts, te))
		return (CMD_OP);
	while (ts < te)
		ts++;
	if (ts == te && *ts != '(')
		return (ARG);
	return (TKN_NONE);
}

t_tokens	peek(char *ps)
{
	return (get_token(&ps, NULL, NULL));
}

bool	peek_consecutive(char *ps, char *charset, char *filter)
{
	if (!ps)
		return (false);
	while (*ps)
	{
		if (ft_strchr(charset, *ps))
			return (true);
		if (ft_strchr(SEP, *ps) || !ft_strchr(filter, *ps))
			break ;
		ps++;
	}
	return (false);
}
