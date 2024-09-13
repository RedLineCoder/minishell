/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:40:24 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/13 18:38:46 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_exec(char **ps, char **pe, char *ts, char *te)
{
	t_execcmd *const	exec = ft_calloc(sizeof(t_execcmd), 1);
	t_cmd				*cmd;
	t_list				*item;
	t_tokens			token;

	((void)ts, (void)te);
	if (!exec)
		return (NULL);
	token = peek(*ps, *pe);
	if (!(token == ARG || token == REDIR))
		return (free(exec), NULL);
	exec->type = EXEC;
	if (pipe(exec->fd) == -1)
		return (free(exec), NULL);
	while (token == ARG || token == REDIR)
	{
		cmd = parse_cmd(ps, pe);
		item = ft_lstnew(cmd);
		if (!cmd || !item)
			return (free(exec), free(cmd), free(item), NULL);
		if (cmd->type == ARG)
			ft_lstadd_back(&exec->args, item);
		else if (cmd->type == REDIR)
			ft_lstadd_back(&exec->redirs, item);
		token = peek(*ps, *pe);
	}
	return ((t_cmd *)exec);
}
