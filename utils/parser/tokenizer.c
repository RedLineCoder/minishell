/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/09/11 08:17:05 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	peek(char *ps, char *charset)
{
	if (!ps)
		return (false);
	if (charset)
		return ((bool)ft_strnstr(ps, charset, ft_strlen(ps)));
	while (*ps)
		if (!ft_strchr(SPACE, *(ps++)))
			return (true);
	return (false);
}

bool	peek_consecutive(char *ps, char *charset)
{
	if (!ps)
		return (false);
	while (*ps)
	{
		if (ft_strchr(charset, *ps))
			return (true);
		ps++;
	}
	return (false);
}


int		get_quote(char	**qs)
{
	int		quotes[2];
	char	*str;
	
	quotes[QUOTE_SINGLE] = 0;
	quotes[QUOTE_DOUBLE] = 0;
	str = *qs;
	while (str && *str)
	{
		if (!quotes[QUOTE_DOUBLE] && *str == '\'')
		{
			if (++quotes[QUOTE_SINGLE] == 2)
				break;
		}
		else if (!quotes[QUOTE_SINGLE] && *str == '"')
			if (++quotes[QUOTE_DOUBLE] == 2)
				break;
		str++;
	}
	*qs = str;
	if (quotes[QUOTE_DOUBLE] == 2)
		return QUOTE_DOUBLE;
	return QUOTE_SINGLE;
}

void	get_operator(char **ps)
{
	char	*str;

	str = *ps;
	while (*str && !ft_strchr(OPERATOR, *str))
		str++;
	// Handle fd error for redirects
	while (*str && ft_strchr(OPERATOR, *str))
		str++;
	*ps = str;
}
/*
t_tokens	get_operator(char *ps, char **ts, char **te)
{
	char	*str;

	str = *ps;
	if (ts)
		*ts = str;
	while (*str && !ft_strchr(OPERATOR, *str))
		str++;
	// Handle fd error for redirects
	while (*str && ft_strchr(OPERATOR, *str))
		str++;
	if (te)
		*te = str;
	*ps = str;
	if (is_redir(*ts, *te) || is_append(*ts, *te))
		return (REDIR);
	else if (is_hdoc(*ts, *te))
		return (HDOC);
	else if (!ft_strncmp(*ts, "|", *te - *ts))
		return (PIPE);
	else if (is_cond(*ts, *te))
		return (COND);
	else
		return (TKN_NONE);
}
*/

t_tokens	get_token(char **ps, char **ts, char **te)
{
	char	*str;

	if (!ps || !*ps || !peek(*ps, NULL))
		return (TKN_NONE);
	while (**ps && ft_strchr(SPACE, **ps))
		(*ps)++;
		/*
	if (ft_strchr(OPERATOR, **ps) || peek(*ps, REDIRS))
	{
		return (get_operator(ps, ts, te));
	} */
	str = *ps;
	if (ts)
		*ts = str;
	while (*str && !ft_strchr(SPACE, *str))
	{
		if (ft_strchr(QUOTES, *str))
			get_quote(&str);
		else if(ft_strchr(BLOCKS, *str))
		{
			str++;
			break;
		}
		else if (ft_strchr(OPERATOR, **ps))
		{
			get_operator(&str);
			break;
		}
		str++;
	}
	if (te)
		*te = str;
	*ps = str;
	if (is_redir(*ts, *te) || is_append(*ts, *te))
		return (REDIR);
	else if (is_hdoc(*ts, *te))
		return (HDOC);
	else if (!ft_strncmp(*ts, "|", *te - *ts))
		return (PIPE);
	else if (is_cond(*ts, *te))
		return (COND);
	else if (is_block(*ts, *te))
		return (BLOCK);
	else
		return (ARG);
}
