/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:17:19 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/11 00:20:12 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*parse_redir(char **ps)
{
	t_redir	*const redir = ft_calloc(sizeof(t_redir), 1);

	if (!redir)
		return (NULL);
	if (**ps == '>');
}
