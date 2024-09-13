/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmdop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:54:46 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/13 02:57:41 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_cmdop(char **ps, char **pe, char *ts, char *te)
{
	t_opcmd *const	op = ft_calloc(sizeof(t_opcmd), 1);

	if (!op)
		return (NULL);
	(void)ps;
	(void)pe;
	op->type = CMD_OP;
	op->op_type = get_cmdop(ts, te);
	return ((t_cmd *)op);
}
