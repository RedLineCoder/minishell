/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_block.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:13:01 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/13 03:32:54 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_binode	*parse_block(char **ps, char **pe, char *ts, char *te)
{
	t_binode *const	block = parser(ts + 1, te, 1);

	(void)ps;
	(void)pe;
	if (!block)
		return (NULL);
	block->cmd->type = BLOCK;
	return (block);
}
