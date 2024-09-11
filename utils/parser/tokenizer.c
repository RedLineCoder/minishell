/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/09/11 14:12:23 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Alternative function
/* int		get_quote(char	**qs)
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
} */

void	get_quote(char	**qs)
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
	// Handle fd error for redirects
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
	else if (ft_strchr(OPERATOR, *str) || peek_consecutive(str, REDIRS))
		get_operator(&str);
	else
		while (*str && !ft_strchr(SEP, *str))
		{
			if (ft_strchr(QUOTES, *str))
				get_quote(&str);
			str++;
		}
	if (te)
		*te = str;
	*ps = str;
}

/*
	Now everything is clear as a pure glass
*/

t_tokens	get_token(char **ps, char **ts, char **te)
{
	if (!ps || !*ps || !peek(*ps, NULL))
		return (TKN_NONE);
	while (**ps && ft_strchr(SPACE, **ps))
		(*ps)++;
	handle_sep(ps, ts, te);
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
// (((ls|3>cat)))