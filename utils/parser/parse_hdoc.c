/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hdoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:35:16 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 13:28:20 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_hdoc(char **ps, char *ts, char *te)
{
	t_hdoccmd *const	hdoc = ft_calloc(sizeof(t_hdoccmd), 1);
	char				*fd;

	if (!hdoc)
		return (NULL);
	hdoc->type = HDOC;
	fd = ft_strndup(ts, te - ts);
	while (!ft_strchr("<>", *ts))
		ts++;
	hdoc->fd = STDIN_FILENO;
	if (*fd != *ts)
		hdoc->fd = ft_atoi(fd);
	get_token(ps, &hdoc->s_limit, &hdoc->e_limit);
	return ((t_cmd *) hdoc);
}
