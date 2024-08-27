/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/08/27 19:29:34 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parser(char *prompt)
{
	char	*ps;
	char	*ts;
	char	*te;
	
	ps = prompt;
	while (1)
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
		if (!peek(ps, "||") || !peek(ps, "&&") || !peek(ps, "|"))
			break;
	}
	//printf("%s\n", prompt);
}
