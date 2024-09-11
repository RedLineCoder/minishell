/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 14:43:05 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parser(char *prompt)
{
	t_binode	*root = ft_calloc(sizeof(t_binode), 1);	
	char	*ps;
	
	if (!root)
		return ;
	ps = prompt;
	while (peek(ps, NULL))
	{
		t_cmd	*cmd = parse_cmd(&ps);
		(void)cmd;
	}
}
