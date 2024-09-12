/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 17:42:29 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 12:49:00 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_arg(char **ps, char *ts, char *te)
{
	t_argcmd *const	arg = ft_calloc(sizeof(t_argcmd), 1);

	(void)ps;
	arg->type = ARG;
	arg->s_arg = ts;
	arg->e_arg = te;
	return ((t_cmd *) arg);
}
