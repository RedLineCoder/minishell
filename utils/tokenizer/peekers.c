/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   peekers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:48:04 by moztop            #+#    #+#             */
/*   Updated: 2024/09/12 21:46:59 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_tokens	peek(char	*ps)
{
	return (get_token(&ps, NULL, NULL));
}

bool	peek_consecutive(char *ps, char *charset, char *filter)
{
	if (!ps)
		return (false);
	while (*ps)
	{
		if (ft_strchr(charset, *ps))
			return (true);
		if (ft_strchr(SEP, *ps) || !ft_strchr(filter, *ps))
			break ;
		ps++;
	}
	return (false);
}
