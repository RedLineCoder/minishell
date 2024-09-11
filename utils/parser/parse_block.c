/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_block.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:13:01 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 16:16:38 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_block(char **ps, char *ts, char *te)
{
	t_blockcmd	*const block = ft_calloc(sizeof(t_blockcmd), 1);
	
	if (!block)
		return NULL;
	while (peek_next(*ps) != BLOCK)
	{
		
	}
	get_token(ps, NULL, NULL);
}