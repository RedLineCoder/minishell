/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validater.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:39:03 by moztop            #+#    #+#             */
/*   Updated: 2024/09/15 16:08:53 by moztop           ###   ########.fr       */
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

int	token_missed(char *ps)
{
	t_tokens	token;
	char		*ts;
	char		*te;

	token = 1;
	while (token)
	{
		token = get_token(&ps, &ts, &te);
		if (token == CMD_OP && !peek(ps))
			return (0);
	}
	return (1);
}

void	print_errsyntax(char *ts, char *te, char *ps, int next)
{
	char	*token;

	if (next)
		get_token(&ps, &ts, &te);
	token = ft_strndup(ts, te - ts);
	ft_putstr_fd(ERR_TKN, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
	free(token);
}

int	err_syntax(char *ps)
{
	t_tokens	token;
	char		*ts;
	char		*te;

	while (1)
	{
		token = get_token(&ps, &ts, &te);
		if (token == CMD_OP || token == BLK_CLS)
			return (print_errsyntax(ts, te, ps, 0), 258);
		while (token)
		{
			if (token == BLK_OP || token == CMD_OP || !token)
				break ;
			if (token == ARG && (peek(ps) == ARG || peek(ps) == REDIR))
				continue ;
			if (token == REDIR && peek(ps) != ARG)
				return (print_errsyntax(ts, te, ps, 1), 258);
			/* if (token == BLK_CLS && (peek(ps) != CMD_OP && peek(ps)))
				return (print_errsyntax(ts, te, ps, 0), 258); */
			token = get_token(&ps, &ts, &te);
		}
		if (peek(ps) == TKN_NONE)
			break;
	}
	return (0);
}
