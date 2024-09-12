/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:50:10 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 22:25:11 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_cmd(char **ps)
{
	char			*ts;
	char			*te;
	t_tokens		token;
	t_cmd			*(*funcs[6])(char **, char *, char *);

	ft_memset(funcs, 0, sizeof(t_cmd *) * 6);
	funcs[REDIR] = parse_redir;
	funcs[CMD_OP] = parse_cmdop;
	funcs[ARG] = parse_arg;
	token = get_token(ps, &ts, &te);
	if (funcs[token])
		return (funcs[token](ps, ts, te));
	return (NULL);
}
