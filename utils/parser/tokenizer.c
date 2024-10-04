/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/10/04 14:23:06 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	pass_block(char *bs, char **be, char *pe)
{
	int		sem_block;

	if (peek(bs, pe, TKN_NONE) != BLK_OP)
		return (1);
	while ((bs != pe) && ft_strchr(SPACE, *bs))
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

	if (ft_strchr(quotes, **qs))
	{
		quote = **qs;
		(*qs)++;
		while (*qs != pe && **qs != quote)
			(*qs)++;
	}
	if (!**qs)
		return (0);
	return (1);
}

int	get_operator(char **te)
{
	char	*str;

	str = *te;
	if (ft_isdigit(*str))
	{
		while (ft_isdigit(*str))
			str++;
		if (!ft_strchr(REDIRS, *str))
			return (0);
	}
	else if (!ft_strchr(OPERATOR, *str))
		return (0);
	if (*str == *(str + 1))
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
	while ((*ps != *pe) && ft_strchr(SPACE, **ps))
		(*ps)++;
	if (*ps == *pe)
		return (TKN_NONE);
	start = *ps;
	if (ft_strchr(BLOCKS, **ps))
		(*ps)++;
	else if (get_operator(ps))
		;
	else
		while (*ps != *pe && !ft_strchr(SEP, **ps))
			if (pass_quote(ps, *pe, QUOTES))
				(*ps)++;
	end = *ps;
	if (ts)
		*ts = start;
	if (te)
		*te = end;
	return (get_token_type(start, end));
}
