/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/09/22 14:28:10 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*pass_quote(char *qs, char *pe)
{
	char	quote;

	if (!ft_strchr(QUOTES, *qs))
		return(qs);
	quote = *qs;
	qs++;
	while (qs != pe && *qs != quote)
		qs++;
	return (qs);
}

void	get_operator(char **te)
{
	while (ft_isdigit(**te))
		*te += 1;
	if (**te == *(*te + 1))
		*te += 1;
	*te += 1;
}

t_tokens	handle_sep(char **ps, char **pe, char **ts, char **te)
{
	char	*str;

	str = *ps;
	if (ts)
		*ts = str;
	if (ft_strchr(BLOCKS, *str))
		str++;
	else if (ft_strchr(OPERATOR, *str) || peek_consecutive(str, *pe, REDIRS, DIGITS))
		get_operator(&str);
	else
	{
		while (str != *pe && !ft_strchr(SEP, *str))
		{
			str = pass_quote(str, *pe);
			str++;
		}
	}
	if (te)
		*te = str;
	*ps = str;
	return (TKN_IN);
}

t_tokens	get_token(char **ps, char **pe, char **ts, char **te)
{
	char	*start;
	char	*end;

	if ((!ps || !*ps || !pe))
		return (TKN_NONE);
	while ((*ps != *pe) && ft_strchr(SPACE, **ps))
		(*ps)++;
	if (*ps == *pe)
		return (TKN_NONE);
	handle_sep(ps, pe, &start, &end);
	if (ts)
		*ts = start;
	if (te)
		*te = end;
	return (get_token_type(start, end));
}
