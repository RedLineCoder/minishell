/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:40:24 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 12:30:05 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_exec(char **ps, char *ts, char *te)
{
	t_execcmd *const	exec = ft_calloc(sizeof(t_execcmd), 1);
	t_cmd				*cmd;
	t_list				*item;
	t_tokens			token;

	(void)ts;
	(void)te;
	if (!exec)
		return (NULL);
	exec->type = EXEC;
	token = peek_next(*ps);
	while (token == ARG || token == REDIR || token == HDOC)
	{
		cmd = parse_cmd(ps);
		if (!cmd)
			return (NULL);
		item = ft_lstnew(cmd);
		if (!item)
			return (NULL);
		if (cmd->type == ARG)
			ft_lstadd_back(&exec->args, item);
		else if (cmd->type == REDIR)
			ft_lstadd_back(&exec->redirs, item);
		else if (cmd->type == HDOC)
			ft_lstadd_back(&exec->hdocs, item);
		token = peek_next(*ps);
	}
	return ((t_cmd *)exec);
}
