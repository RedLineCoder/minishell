/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:17:19 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 13:17:27 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"



t_cmd	*parse_redir(char **ps, char *ts, char *te)
{
	t_redircmd	*const	redir = ft_calloc(sizeof(t_redircmd), 1);
	char				*fd;
	
	if (!redir)
		return (NULL);
	redir->type = REDIR;
	fd = ft_strndup(ts, te - ts);
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
	get_token(ps, &redir->s_file, &redir->e_file);
	return ((t_cmd *) redir);		
}