/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cond.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:54:46 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 17:11:13 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_cond(char **ps, char *ts, char *te)
{
	t_condcmd	*const cond = ft_calloc(sizeof(t_condcmd), 1);

	if (!cond)
		return (NULL);
	(void)ps;
	cond->type = COND;
	cond->cond_type = COND_OR;
	if (ft_strncmp(ts, "&&", te - ts))
		cond->cond_type = COND_AND;
	return ((t_cmd *)cond);	
}