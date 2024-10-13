/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 13:57:43 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/12 15:31:12 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tree_map(t_cmd *cmd, void *payload, int (*f)(t_cmd *, void *))
{
	t_tokens const	token = cmd->type;
	t_list			*lst;

	if (!f(cmd, payload))
		return (false);
	if (token == PIPE)
	{
		lst = ((t_pipecmd *)cmd)->pipelist;
		while (lst)
		{
			if (!tree_map(lst->content, payload, f))
				return (false);
			lst = lst->next;
		}
	}
	else if (token == LOGIC
		&& (!tree_map(((t_logiccmd *)cmd)->left, payload, f)
			|| !tree_map(((t_logiccmd *)cmd)->right, payload, f)))
		return (false);
	else if (token == SUBSHELL && !tree_map(((t_blockcmd *)cmd)->subshell, payload, f))
		return (false);
	return (true);
}
