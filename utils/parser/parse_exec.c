/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:40:24 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 14:40:31 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_exec(char **ps, char *ts, char *te)
{
	t_execcmd	*const exec = ft_calloc(sizeof(t_execcmd), 1);
	t_cmd		*cmd;
	if (!exec)
		return (NULL);
	while (!(peek(*ps, "||") || peek(*ps, "&&") || peek(*ps, "|")))
	{
				
	}
	return (exec);
}