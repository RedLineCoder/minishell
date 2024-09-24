/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 01:55:34 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/23 02:05:42 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	destroy_pipes(int	**pipes, int size)
{
	while (--size >= 0)
	{
		close_pipe(pipes[size]);
		free(pipes[size]);
	}
	free(pipes);
}

int		init_pipes(t_pipecmd *pipecmd)
{
	int			i;
	const int	pipe_count = ft_lstsize(pipecmd->pipelist);

	pipecmd->pipes = ft_calloc(sizeof(int *), pipe_count);
	if (!pipecmd->pipes)
		return (0);
	i = -1;
	while (++i < pipe_count)
	{
		pipecmd->pipes[i] = ft_calloc(sizeof(int) * 2, 1);
		if (!pipecmd->pipes[i])
			return (destroy_pipes(pipecmd->pipes, i), 0);
		if (pipe(pipecmd->pipes[i]) == -1)
			return (free(pipecmd->pipes[i]), destroy_pipes(pipecmd->pipes, i), 0);
	}	
	return (1);
}