/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:08 by moztop            #+#    #+#             */
/*   Updated: 2024/09/28 15:10:29 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_tree(void *cmd)
{
	if (!cmd)
		return ;
	else if (((t_cmd *)cmd)->type == LOGIC)
	{
		clean_tree(((t_logiccmd *)cmd)->left);
		clean_tree(((t_logiccmd *)cmd)->right);
	}
	else if (((t_cmd *)cmd)->type == PIPE)
		ft_lstclear(&((t_pipecmd *)cmd)->pipelist, clean_tree);
	else if (((t_cmd *)cmd)->type == SUBSHELL)
	{
		clean_tree(((t_blockcmd *)cmd)->subshell);
		ft_lstclear(&((t_blockcmd *)cmd)->redirs, clean_tree);
	}
	else if (((t_cmd *)cmd)->type == EXEC)
	{
		ft_lstclear(&((t_execcmd *)cmd)->redirs, clean_tree);
		ft_lstclear(&((t_execcmd *)cmd)->args, free);
	}
	free(cmd);
}
