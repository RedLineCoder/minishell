/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validater.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:39:03 by moztop            #+#    #+#             */
/*   Updated: 2024/09/15 08:31:37 by moztop           ###   ########.fr       */
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
	t_tokens	prev;
	char		*ts;
	char		*te;

	token = get_token(&ps, &ts, &te);
	if (token && !peek(ps))
		return (1);
	while (token)
	{
		prev = token;
		token = get_token(&ps, &ts, &te);
		if (token != CMD_OP || prev != ARG)
			continue ;
		if (!peek(ps))
			return (0);
	}
	return (1);
}

void	print_errsyntax(char *ts, char *te, int newline)
{
	char	*token;

	if (newline)
	{
		ft_putstr_fd(ERR_TKN, 2);
		ft_putstr_fd("'", 2);
		ft_putstr_fd("newline", 2);
		ft_putendl_fd("'", 2);
	}
	else
	{
		token = ft_strndup(ts, te - ts);
		ft_putstr_fd(ERR_TKN, 2);
		ft_putstr_fd("'", 2);
		ft_putstr_fd(token, 2);
		ft_putendl_fd("'", 2);
		free(token);
	}
}

int	err_syntax(char *ps)
{
	t_tokens	token;
	t_tokens	prev;
	char		*str;
	char		*ts;
	char		*te;

	(void)prev;
	str = NULL;
	token = get_token(&ps, &ts, &te);
	if (token && !peek(ps) && !(token == ARG || token == REDIR || *ts == '('))
		return (print_errsyntax(ts, te, 0), 258);
	if (token == REDIR && !peek(ps))
		return (print_errsyntax(ts, te, 1), 258);
	while (token)
	{
		prev = token;
		token = get_token(&ps, &ts, &te);
		if (prev == CMD_OP && !(token == ARG || token == REDIR || *ts == '('))
			return (print_errsyntax(ts, te, 0), 258);
		if (*ts == '(' && !(prev == ARG || prev == REDIR))
			return (print_errsyntax(ts, te, 0), 258);
		if (prev == REDIR && token != ARG)
			return (print_errsyntax(ts, te, 0), 258);
	}
	return (0);
}
