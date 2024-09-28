/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:08 by moztop            #+#    #+#             */
/*   Updated: 2024/09/28 12:41:34 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_node(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == LOGIC)
	{
		clean_node(((t_logiccmd *)cmd)->left);
		clean_node(((t_logiccmd *)cmd)->right);
	}
	if (cmd->type == PIPE)
		ft_lstclear(&((t_pipecmd *)cmd)->pipelist, clean_node);
	if (cmd->type == SUBSHELL)
		clean_node(((t_blockcmd *)cmd)->subshell);
}