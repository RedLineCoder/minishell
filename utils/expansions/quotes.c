/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:19:16 by moztop            #+#    #+#             */
/*   Updated: 2024/10/11 17:29:05 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	unquoted_size(t_list *explst, char *arg)
{
	int	len;
	int	qd;
	int	qs;

	len = 0;
	qd = 0;
	qs = 0;
	while (*arg)
	{
		if (*arg == '\'' && !qd && !is_expanded(explst, arg))
		{
			arg++;
			qs = !qs;
			continue ;
		}
		if (*arg == '"' && !qs && !is_expanded(explst, arg))
		{
			arg++;
			qd = !qd;
			continue ;
		}
		arg++;
		len++;
	}
	return (len);
}

void	write_unquote_arg(t_list *explst, char *exp, char *arg)
{
	int		qd;
	int		qs;

	qd = 0;
	qs = 0;
	while (*arg)
	{
		if (*arg == '\'' && !qd && !is_expanded(explst, arg))
		{
			arg++;
			qs = !qs;
			continue ;
		}
		if (*arg == '"' && !qs && !is_expanded(explst, arg))
		{
			arg++;
			qd = !qd;
			continue ;
		}
		*exp++ = *arg++;
	}
}

char	*unquote_arg(t_list *explst, char *arg)
{
	char	*exp;

	exp = ft_calloc(sizeof(char), unquoted_size(explst, arg) + 1);
	if (!exp || !arg)
		return (free(exp), free(arg), NULL);
	write_unquote_arg(explst, exp, arg);
	free(arg);
	return (exp);
}
