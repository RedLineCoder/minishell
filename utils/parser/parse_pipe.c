/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 17:00:17 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 17:11:32 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_pipe(char **ps, char *ts, char *te)
{
	t_cmd	*const pipe = ft_calloc(sizeof(t_cmd), 1);

	(void)ps;
	(void)ts;
	(void)te;
	if (!pipe)
		return (NULL);
	pipe->type = PIPE;
	return (pipe);
}