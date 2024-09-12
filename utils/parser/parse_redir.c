/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:17:19 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 12:28:41 by moztop           ###   ########.fr       */
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
	if (is_append(ts, te))
		redir->redir_type = REDIR_APPEND;
	else if (*ts == '>')
		redir->redir_type = REDIR_OUTPUT;
	else if (*ts == '<')
		redir->redir_type = REDIR_INPUT;
	redir->fd = redir->redir_type || 0;
	if (*fd != *ts)
		redir->fd = ft_atoi(fd);
	free(fd);
	if (peek_next(*ps) == ARG)
		get_token(ps, &redir->s_file, &redir->e_file);
	return ((t_cmd *) redir);
}
