/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 06:43:27 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 07:48:12 by moztop           ###   ########.fr       */
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
		t_tokens type = get_token(&ps, &ts, &te);
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
		else if (type == COND)
			printf(" COND");
		else if (type == BLOCK)
			printf(" BLOCK");
		printf("\n");
	}
	//printf("%s\n", prompt);
}
