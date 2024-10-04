/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 13:57:43 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/04 14:15:30 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	tree_map(t_cmd *cmd, int (*f)(t_cmd *))
{
	t_tokens const	token = cmd->type;
	t_list			*lst;
	int				return_val;
	return_val = f(cmd);
	if (token == PIPE)
	{
		lst = ((t_pipecmd *)cmd)->pipelist;
		while (lst)
		{
			tree_map(lst->content, f);
			lst = lst->next;
		}
	}
	else if (token == LOGIC)
	{
		tree_map(((t_logiccmd *)cmd)->left, f);
		tree_map(((t_logiccmd *)cmd)->right, f);
	}
	else if (token == SUBSHELL)
		tree_map(((t_blockcmd *)cmd)->subshell, f);
	return (return_val);
}