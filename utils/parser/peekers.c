/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   peekers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:48:04 by moztop            #+#    #+#             */
/*   Updated: 2024/09/11 14:45:00 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	peek(char *ps, char *charset)
{
	if (!ps)
		return (false);
	if (charset)
		return ((bool)ft_strnstr(ps, charset, ft_strlen(ps)));
	while (*ps)
		if (!ft_strchr(SPACE, *(ps++)))
			return (true);
	return (false);
}

bool	peek_consecutive(char *ps, char *charset)
{
	if (!ps)
		return (false);
	while (*ps)
	{
		if (ft_strchr(charset, *ps))
			return (true);
		if (ft_strchr(SEP, *ps))
			break ;
		ps++;
	}
	return (false);
}
