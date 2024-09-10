/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 00:16:11 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parser(char *prompt)
{
	t_binode	*root = ft_calloc(sizeof(t_binode), 1);	
	char	*ps;
	char	*ts;
	char	*te;
	
	if (!root)
		return ;
	ps = prompt;
	while (peek(ps, NULL))
	{
		t_cmds type = get_token(&ps, &ts, &te);
		while (ts < te)
		{
			printf("%c", *ts);
			ts++;
		}
		if (type == PIPE)
			printf(" PIPE");
		else if(type == REDIR)
			printf(" REDIR");
		else if (type == HDOC)
			printf(" HDOC");
		else if (type == ARG)
			printf(" ARG");
		printf("\n");
	}
	//printf("%s\n", prompt);
}
