/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:50:10 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 12:48:07 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_cmd(char **ps)
{
	char			*ts;
	char			*te;
	t_tokens		token;
	t_cmd			*(*funcs[7])(char **, char *, char *);

	ft_memset(funcs, 0, sizeof(t_cmd *) * 7);
	funcs[REDIR] = parse_redir;
	funcs[HDOC] = parse_hdoc;
	funcs[ARG] = parse_arg;
	funcs[PIPE] = parse_pipe;
	funcs[COND] = parse_cond;
	token = get_token(ps, &ts, &te);
	if (funcs[token])
		return (funcs[token](ps, ts, te));
	return (NULL);
}
