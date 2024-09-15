/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validater.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:39:03 by moztop            #+#    #+#             */
/*   Updated: 2024/09/15 07:03:21 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	quote_closed(char *ps)
{
	char	quote;

	while (*ps)
	{
		if (*ps == '\'' || *ps == '"')
		{
			quote = *ps;
			ps++;
			while (*ps && *ps != quote)
				ps++;
			if (!*ps)
				return (0);
		}
		ps++;
	}
	return (1);
}

int	block_closed(char *ps)
{
	int		sem_block;

	sem_block = 0;
	while (*ps)
	{
		if (*ps == ')')
			sem_block--;
		if (*ps == '(')
			sem_block++;
		ps++;
	}
	if (sem_block > 0)
		return (0);
	return (1);
}

int	err_syntax(char *ps)
{
	t_tokens	token;
	char		*str;
	char		*ts;
	char		*te;

	str = NULL;
	if (!block_closed(ps))
		return (printf("%s `block'\n", ERR_MISS), 258);
	if (!quote_closed(ps))
		return (printf("%s `quote'\n", ERR_MISS), 258);
	token = 1;
	while (token)
	{
		token = get_token(&ps, &ts, &te);
		if (token == BLOCK)
			return (printf("%s `)'\n", ERR_TKN), 258);
	}
	return (0);
}

/* token = get_token(&ps, &ts, &te);
if (token == CMD_OP && blkst)
{
	str = ft_strndup(ts, te - ts);
	return (printf("%s `%s'\n", ERR_SYNTAX, str), free(str), 258);
}
if (token == REDIR)
{
	if (peek(ps) && peek(ps) != ARG)
	{
		token = get_token(&ps, &ts, &te);
		str = ft_strndup(ts, te - ts);
		return (printf("%s `%s'\n", ERR_SYNTAX, str), free(str), 258);
	}
	else
		return (printf("%s `%s'\n", ERR_SYNTAX, "newline"), 258);
}
if (*ts == '(')
	return (err_syntax(ps));
if (*ts == ')')
	return (printf("%s `%s'\n", ERR_SYNTAX, str), free(str), 258);
if (token == ARG)
	blkst = 0; */