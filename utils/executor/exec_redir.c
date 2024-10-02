/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 09:47:20 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/02 19:01:19 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void exec_hdoc(t_redircmd *redir)
{
	char			*buffer;
	const size_t	len = redir->e_spec - redir->s_spec;

	if (pipe(redir->pipe) == -1)
		return ; 
	while (1)
	{
		buffer = readline("> ");
		if (!buffer || (ft_strlen(buffer) == len && !ft_strncmp(buffer, redir->s_spec, len)))
		{
			free(buffer);
			close(redir->pipe[1]);
			return ;
		}
		write(redir->pipe[1], buffer, ft_strlen(buffer));
		write(redir->pipe[1], "\n", 1);
		free(buffer);
	}
}

int	execute_redir(t_redircmd *redir)
{
	int				fd;
	char			*file;
	int				flags;
	t_redir	const	type = redir->redir_type;

	if (type == REDIR_HDOC)
	{
		dup2(redir->pipe[0], redir->fd);
		close(redir->pipe[0]);
		return (1);
	}
	flags = O_RDWR;
	if (type == REDIR_APPEND)
		flags |= O_APPEND;
	if (type != REDIR_INPUT)
		flags |= O_CREAT;
	file = ft_strndup(redir->s_spec, redir->e_spec - redir->s_spec);
	fd = open(file, flags, S_IRWXU);
	free(file);
	if (fd == -1)
		return (0);
	if (type == REDIR_APPEND || type == REDIR_OUTPUT)
		dup2(fd, redir->fd);
	else
		dup2(fd, redir->fd);
	close(fd);
	return (1);
}