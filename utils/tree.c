/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 13:57:43 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/29 10:06:32 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tree_map(t_cmd *cmd, void *payload, int (*f)(t_cmd *, void *))
{
	int const	token = cmd->type;
	t_list		*lst;

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
	else if (token == SUBSHELL
		&& !tree_map(((t_blockcmd *)cmd)->subshell, payload, f))
		return (false);
	return (true);
}

void	clean_tree(void *cmd)
{
	t_cmdtype	token;

	if (!cmd)
		return ;
	token = ((t_cmd *)cmd)->type;
	if (token == EXEC)
	{
		ft_lstclear(&((t_execcmd *)cmd)->args, free);
		ft_lstclear(&((t_execcmd *)cmd)->redirs, clean_tree);
	}
	else if (token == REDIR)
		ft_lstclear(&((t_redircmd *)cmd)->args, free);
	else if (token == SUBSHELL)
	{
		ft_lstclear(&((t_blockcmd *)cmd)->redirs, clean_tree);
		clean_tree(((t_blockcmd *)cmd)->subshell);
	}
	else if (token == PIPE)
		ft_lstclear(&((t_pipecmd *)cmd)->pipelist, clean_tree);
	else if (token == LOGIC)
	{
		clean_tree(((t_logiccmd *)cmd)->left);
		clean_tree(((t_logiccmd *)cmd)->right);
	}
	free(cmd);
}
