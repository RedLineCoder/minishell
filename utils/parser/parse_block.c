/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_block.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:13:01 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/12 12:48:49 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_block(char **ps, char *ts, char *te)
{
	t_blockcmd *const	block = ft_calloc(sizeof(t_blockcmd), 1);

	if (!block)
		return (NULL);
	while (peek_next(*ps) != BLOCK)
		;
	get_token(ps, NULL, NULL);
}
