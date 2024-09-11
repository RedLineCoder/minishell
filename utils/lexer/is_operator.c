/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_operator.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:19:57 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 14:16:20 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_redir(char *ts, char *te)
{
	while (ts < te && !ft_strchr(REDIRS, *ts))
		ts++;
	return ((te - 1 == ts) && ft_strchr(REDIRS, *ts));
}

int	is_block(char *ts, char *te)
{
	return ((ts == te - 1) && ft_strchr(BLOCKS, *ts));
}

int	is_hdoc(char *ts, char *te)
{
	int	count;

	count = 0;
	while (ts < te && !ft_strchr(REDIRS, *ts))
		ts++;
	while (ts < te && *ts == '<')
	{
		count++;
		ts++;
	}
	return (count == 2);
}

int	is_append(char *ts, char *te)
{
	int	count;

	count = 0;
	while (ts < te && !ft_strchr(REDIRS, *ts))
		ts++;
	while (ts < te && *ts == '>')
	{
		count++;
		ts++;
	}
	return (count == 2);
}

int	is_cond(char *ts, char *te)
{
	int	count;

	count = 0;
	while (ts < te && *ts == '|')
	{
		count++;
		ts++;
		if (*ts != '|')
			;
		// handle syntax error
	}
	while (ts < te && *ts == '&')
	{
		count++;
		ts++;
		if (*ts != '&')
			;
		// handle syntax error
	}
	return (count == 2);
}
