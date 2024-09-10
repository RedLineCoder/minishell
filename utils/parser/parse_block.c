/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_block.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 23:54:09 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 00:11:10 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*parse_block(char **ps)
{
	t_blockcmd	*const	block = ft_calloc(sizeof(t_blockcmd), 1);
	t_binode	*const	node = ft_calloc(sizeof(t_binode), 1);
 	if (!block)
		return (0);
	if (!node)
		return (0);
	while (peek(*ps, NULL))
	{
		if (peek(*ps, ">") || peek(*ps, "<"))
			parse_redir(ps);
		if (**ps == ')')
			break;	
	}
	if (!**ps)
		return 0;
	block->type = BLOCK;
	return (block);
}

t_cmd	*parsecmd(char **ps, char **pe)
{
	
}