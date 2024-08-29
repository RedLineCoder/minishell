/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/08/29 20:12:57 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	peek(char *ps, char *charset)
{
	if (charset)
		return ((bool) ft_strnstr(ps, charset, ft_strlen(ps)));
	while (*ps)
	{
		if (!ft_strchr(ARGSEP, *ps))
			return true;
		ps++;
	}
	return false;
}

t_cmds	get_token(char **ps, char **ts, char **te)
{
	char	*str;
	
	if (!ps || !*ps)
		return CMD_NONE;
	str = *ps;
	while (*str && ft_strchr(ARGSEP, *str))
		str++;
	if (ts)
		*ts = str;
	while (*str && !ft_strchr(ARGSEP, *str))
		str++;
	if (te)
		*te = str;
	*ps = str;
	
	if (!ft_strncmp(*ts, "<<", 2))
		return HDOC;
	else if (!ft_strncmp(*ts, "<", 1) || !ft_strncmp(*ts, ">", 1))
		return REDIR;
	else if (!ft_strncmp(*ts, "|", 1))
		return PIPE;
	else if (!ft_strncmp(*ts, "&&", 2) || !ft_strncmp(*ts, "||", 2))
		return COND;
	else if (!ft_strncmp(*ts, "$", 1))
		return DOLLAR;
	else 
		return ARG;
}