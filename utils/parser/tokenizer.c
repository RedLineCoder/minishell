/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/10/08 19:14:44 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	pass_block(char *bs, char **be, char *pe)
{
	int		sem_block;

	if (peek(bs, pe, TKN_NONE) != BLK_OP)
		return (1);
	while ((bs != pe) && str_include(SPACE, *bs))
		bs++;
	sem_block = 1;
	while (bs != pe && sem_block)
	{
		bs++;
		if (*bs == ')')
			sem_block--;
		if (*bs == '(')
			sem_block++;
	}
	if (!*bs)
		return (0);
	return (*be = bs, 1);
}

int	pass_quote(char **qs, char *pe, char *quotes)
{
	char	quote;
	int		size;

	size = 0;
	if (str_include(quotes, **qs))
	{
		quote = *(*qs)++;
		size++;
		while (*qs != pe && **qs != quote)
		{
			(*qs)++;
			size++;
		}
		if (**qs)
			size++;
	}
	if (!**qs)
		return (-1);
	return (size);
}

int	get_operator(char **te)
{
	char	*str;

	str = *te;
	if (ft_isdigit(*str))
	{
		while (ft_isdigit(*str))
			str++;
		if (!str_include(REDIRS, *str))
			return (0);
	}
	else if (!str_include(OPERATOR, *str))
		return (0);
	if (*(str + 1) && *str == *(str + 1))
		str++;
	*te = str + 1;
	return (1);
}

t_tokens	get_token(char **ps, char **pe, char **ts, char **te)
{
	char	*start;
	char	*end;

	if ((!ps || !pe) || (ps == pe))
		return (TKN_NONE);
	while ((*ps != *pe) && str_include(SPACE, **ps))
		(*ps)++;
	if (*ps == *pe)
		return (TKN_NONE);
	start = *ps;
	if (str_include(BLOCKS, **ps))
		(*ps)++;
	else if (get_operator(ps))
		;
	else
		while (*ps != *pe && !str_include(SEP, **ps))
			if (pass_quote(ps, *pe, QUOTES) != -1)
				(*ps)++;
	end = *ps;
	if (ts)
		*ts = start;
	if (te)
		*te = end;
	return (get_token_type(start, end));
}
