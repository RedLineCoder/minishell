/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 17:42:29 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/13 02:57:58 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_arg(char **ps, char **pe, char *ts, char *te)
{
	t_argcmd *const	arg = ft_calloc(sizeof(t_argcmd), 1);

	if (!arg)
		return (NULL);
	(void)ps;
	(void)pe;
	arg->type = ARG;
	arg->s_arg = ts;
	arg->e_arg = te;
	return ((t_cmd *) arg);
}
