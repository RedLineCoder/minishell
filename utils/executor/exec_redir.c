/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 09:47:20 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/15 12:59:55 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void exec_hdoc(t_execcmd *cmd, t_redircmd *redir)
{
	char			*buffer;
	const size_t	len = redir->e_spec - redir->s_spec;
	
	close_pipe(cmd->fd);
	if (pipe(cmd->fd) == -1)
		return ;
	cmd->in_file = cmd->fd[0];
	while (1)
	{
		buffer = readline("> ");
		if (ft_strlen(buffer) == len && !ft_strncmp(buffer, redir->s_spec, len))
		{
			free(buffer);
			close(cmd->fd[1]);
			return ;
		}
		write(cmd->fd[1], buffer, ft_strlen(buffer));
		write(cmd->fd[1], "\n", 1);
		free(buffer);
	}
}

int	execute_redir(t_execcmd *cmd, t_redircmd *redir)
{
	int				fd;
	char			*file;
	int				flags;
	t_redir	const	type = redir->redir_type;

	if (type == REDIR_HDOC)
	{
		cmd->in_file = cmd->fd[0];
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
		cmd->out_file = fd;
	else
		cmd->in_file = fd;
	return (1);
}

int	exec_pipe(t_execcmd	*cmd)
{
	close_pipe(cmd->fd);
	if (pipe(cmd->fd) == -1)
		return (0);
	cmd->out_file = cmd->fd[1];
	return (1);
}