/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_operator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:19:57 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 23:02:04 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_redir	get_redir(char *ts, char *te)
{
	int	len;

	while (ts < te && !ft_strchr(REDIRS, *ts))
		ts++;
	len = te - ts;
	if (!len)
		return (REDIR_NONE);
	if (!ft_strncmp(ts, ">", len))
		return (REDIR_OUTPUT);
	if (!ft_strncmp(ts, "<", len))
		return (REDIR_INPUT);
	if (!ft_strncmp(ts, ">>", len))
		return (REDIR_APPEND);
	if (!ft_strncmp(ts, "<<", len))
		return (REDIR_HDOC);
	return (REDIR_NONE);
}

t_cmdop	get_cmdop(char *ts, char *te)
{
	size_t	len;

	len = te - ts;
	if (!len)
		return (OP_NONE);
	if (!ft_strncmp(ts, "|", len))
		return (OP_PIPE);
	if (!ft_strncmp(ts, "||", len))
		return (OP_OR);
	if (!ft_strncmp(ts, "&&", len))
		return (OP_AND);
	return (OP_NONE);
}
