/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:36:27 by moztop            #+#    #+#             */
/*   Updated: 2024/09/11 00:34:57 by emyildir         ###   ########.fr       */
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
		if (!ft_strchr(" ", *(ps++)))
			return (true);
	return (false);
}

t_cmds	get_token(char **ps, char **ts, char **te)
{
	char	*str;

	if (!ps || !*ps || !peek(*ps, NULL))
		return (CMD_NONE);
	str = *ps;
	while (*str && ft_strchr(" ", *str))
		str++;
	if (ts)
		*ts = str;
	while (*str && !ft_strchr(" ", *str))
		str++;
	if (te)
		*te = str;
	*ps = str;
	if (is_redir(*ts, *te)
	|| !ft_strncmp(*ts, ">>", *te - *ts))
		return REDIR;
	else if (!ft_strncmp(*ts, "<<", *te - *ts))
		return HDOC;
	else if(!ft_strncmp(*ts, "|", *te - *ts))
		return PIPE;
	else if (!ft_strncmp(*ts, "||", *te - *ts) ||  !ft_strncmp(*ts, "&&", *te - *ts))
		return COND;
	else if (!ft_strncmp(*ts, "(", *te - *ts))
		return BLOCK;
	else
		return ARG;
}
