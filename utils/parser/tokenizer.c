/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/08/30 18:06:32 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	peek(char *ps, char *charset)
{
	if (charset)
		return ((bool)ft_strnstr(ps, charset, ft_strlen(ps)));
	while (*ps)
	{
		if (!ft_strchr(SPACE, *ps))
			return (true);
		ps++;
	}
	return (false);
}

t_cmds	get_token(char **ps, char **ts, char **te)
{
	char	*str;

	if (!ps || !*ps || !peek(*ps, NULL))
		return (CMD_NONE);
	str = *ps;
	while (*str && ft_strchr(SPACE, *str))
		str++;
	if (ts)
		*ts = str;
	while (*str && !ft_strchr(SPACE, *str))
		str++;
	if (te)
		*te = str;
	*ps = str;
	
}
