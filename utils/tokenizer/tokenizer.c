/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/09/12 22:57:37 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_tokens	get_token_type(char *ts, char *te)
{
	if (ft_strchr(BLOCKS, *ts))
		return (BLOCK);
	else if (get_redir(ts, te))
		return (REDIR);
	else if (get_cmdop(ts, te))
		return (CMD_OP);
	while (ts < te && !ft_strchr(SEP, *ts))
		ts++;
	if (ts == te)
		return (ARG);
	return (TKN_NONE);
}

void	get_quote(char **qs)
{
	char	quote;
	char	*str;

	str = *qs;
	quote = *str++;
	while (str && *str && *str != quote)
		str++;
	*qs = str;
}

void	get_operator(char **pe)
{
	char	*str;
	char	opt;

	str = *pe;
	while (*str && !ft_strchr(OPERATOR, *str))
		str++;
	opt = *str;
	while (*str && *str == opt)
		str++;
	*pe = str;
}

void	handle_sep(char **ps, char **ts, char **te)
{
	char	*str;

	str = *ps;
	if (ts)
		*ts = str;
	if (ft_strchr(BLOCKS, *str))
		str++;
	else if (ft_strchr(OPERATOR, *str) || peek_consecutive(str, REDIRS, DIGITS))
		get_operator(&str);
	else
	{
		while (*str && !ft_strchr(SEP, *str))
		{
			if (ft_strchr(QUOTES, *str))
				get_quote(&str);
			str++;
		}
	}
	if (te)
		*te = str;
	*ps = str;
}

t_tokens	get_token(char **ps, char **ts, char **te)
{
	char	*start;
	char	*end;

	if (!ps || !*ps)
		return (TKN_NONE);
	while (**ps && ft_strchr(SPACE, **ps))
		(*ps)++;
	handle_sep(ps, &start, &end);
	if (ts)
		*ts = start;
	if (te)
		*te = end;
	return (get_token_type(start, end));
}
