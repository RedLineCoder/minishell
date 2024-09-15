/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 02:37:48 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/15 03:31:58 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*get_node_cmd(t_node *node)
{
	if (!node)
		return (NULL);
	return (node->cmd);
}

int	get_node_type(t_node *node)
{	
	t_cmd	*cmd = get_node_cmd(node);
	
	if (!cmd)
		return (TKN_NONE);
	if (cmd->type == REDIR)
		return (((t_redircmd *)cmd)->redir_type);
	else if (cmd->type == CMD_OP)
		return (((t_opcmd *)cmd)->op_type);
	return (node->cmd->type);
}
