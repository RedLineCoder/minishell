/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:17:19 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 21:54:28 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_redir(char **ps, char *ts, char *te)
{
	t_redircmd *const	redir = ft_calloc(sizeof(t_redircmd), 1);
	char				*fd;

	if (!redir)
		return (NULL);
	redir->type = REDIR;
	fd = ft_strndup(ts, te - ts);
	if (!fd)
		return (free(redir), NULL);
	while (!ft_strchr("<>", *ts))
		ts++;
	redir->redir_type = get_redir(ts, te);
	redir->fd = redir->redir_type || 0;
	if (*fd != *ts)
		redir->fd = ft_atoi(fd);
	free(fd);
	if (peek(*ps) == ARG)
		get_token(ps, &redir->s_spec, &redir->e_spec);
	return ((t_cmd *) redir);
}
