/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:58:48 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/21 22:05:38 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_redir_flags(t_redir type)
{
	int		flags;

	flags = 0;
	if (type == REDIR_INPUT)
		flags |= O_RDONLY;
	if (type == REDIR_OUTPUT || type == REDIR_APPEND)
		flags |= O_WRONLY;
	if (type == REDIR_APPEND)
		flags |= O_APPEND;
	if (type != REDIR_INPUT)
		flags |= O_CREAT;
	return (flags);
}

int	handle_redirects(t_list *redirs, t_msh *msh)
{
	while (redirs)
	{
		if (!execute_redir(redirs->content, msh))
			return (false);
		redirs = redirs->next;
	}
	return (true);
}

int	handle_back_redirects(t_list *redirs)
{
	t_redircmd	*redir;

	if (!redirs)
		return (true);
	handle_back_redirects(redirs->next);
	redir = redirs->content;
	if (redir->old_fd != -1)
	{
		if (dup2(redir->old_fd, redir->fd) == -1)
			return (false);
		close(redir->old_fd);
	}
	return (true);
}

int	run_heredoc(t_redircmd *redir, t_msh *msh)
{
	char		*buffer;
	char		*temp;
	const int	expansion = !ft_strchr(redir->args->content, '\"') && 1;
	char *const	eof = unquote_arg(NULL, redir->args->content);

	if (pipe(redir->pipe) == -1)
		return (mini_panic("heredoc", "pipe error", false));
	buffer = readline("> ");
	while (buffer && ft_strncmp(buffer, eof, ft_strlen(eof) + 1))
	{
		temp = buffer;
		if (expansion)
			buffer = expand_dollar(buffer, NULL, msh);
		write(redir->pipe[1], buffer, ft_strlen(buffer));
		write(redir->pipe[1], "\n", 1);
		if (expansion)
			free(temp);
		free(buffer);
		buffer = readline("> ");
	}
	free(eof);
	close(redir->pipe[1]);
	if (!buffer)
		return (mini_panic("heredoc", "readline error.", false));
	return (true);
}
