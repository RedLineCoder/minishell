/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:52:45 by moztop            #+#    #+#             */
/*   Updated: 2024/10/26 18:04:56 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	track_quotes(t_write *wrt, t_list *explst, char *arg)
{
	int const	expanded = !explst || !is_expanded(explst, wrt->a_i);

	if (str_include(QUOTES, arg[wrt->a_i]) && expanded
		&& (wrt->qc == arg[wrt->a_i] || !wrt->qc))
	{
		wrt->qc ^= arg[wrt->a_i];
		return (1);
	}
	return (0);
}

char	*unquote_arg(t_list *explst, char *arg)
{
	t_write *const	wrt = &(t_write){0};
	char			*exp;

	if (!arg)
		return (NULL);
	while (arg[wrt->a_i])
	{
		if (track_quotes(wrt, explst, arg) && (wrt->a_i++, 1))
			continue ;
		wrt->e_i++;
		wrt->a_i++;
	}
	exp = ft_calloc(sizeof(char), wrt->e_i + 1);
	if (!exp)
		return (NULL);
	ft_memset(wrt, 0, sizeof(t_write));
	while (arg[wrt->a_i])
	{
		if (track_quotes(wrt, explst, arg) && (wrt->a_i++, 1))
			continue ;
		exp[wrt->e_i++] = arg[wrt->a_i++];
	}
	return (free(arg), exp);
}
