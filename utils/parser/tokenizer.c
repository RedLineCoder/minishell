/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/09/15 06:54:06 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	get_block(char **bs)
{
	int		sem_block;
	char	*str;

	str = *bs;
	sem_block = 1;
	while (str && *str && sem_block)
	{
		str++;
		if (*str == ')')
			sem_block--;
		if (*str == '(')
			sem_block++;
	}
	*bs = str + 1;
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

	str = *pe;
	while (ft_isdigit(*str))
		str++;
	if (*str == *(str + 1))
		str++;
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
	if (!**ps)
		return (TKN_NONE);
	handle_sep(ps, &start, &end);
	if (ts)
		*ts = start;
	if (te)
		*te = end;
	return (get_token_type(start, end));
}
