/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cond.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:54:46 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 12:47:54 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_cond(char **ps, char *ts, char *te)
{
	t_condcmd *const	cond = ft_calloc(sizeof(t_condcmd), 1);

	if (!cond)
		return (NULL);
	(void)ps;
	cond->type = COND;
	cond->cond_type = COND_OR;
	if (ft_strncmp(ts, "&&", te - ts))
		cond->cond_type = COND_AND;
	return ((t_cmd *)cond);
}
