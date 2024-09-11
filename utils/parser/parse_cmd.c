/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:50:10 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 14:38:44 by emyildir         ###   ########.fr       */
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
	token = get_token(ps, &ts, &te);
	if (funcs[token])
		return (funcs[token](ps, ts, te));
	return NULL;
}