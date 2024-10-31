/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 20:47:31 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/31 12:32:33 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	run_heredoc(t_redircmd *redir, t_msh *msh)
{
	char		*buffer;
	char		*temp;
	const int	expansion = !ft_strchr(redir->args->content, '\"');
	char *const	eof = unquote_arg(NULL, redir->args->content);

	buffer = readline("> ");
	while (buffer && ft_strncmp(buffer, eof, ft_strlen(eof) + 1))
	{
		temp = buffer;
		if (expansion)
			buffer = expand_dollar(buffer, NULL, msh);
		ft_putendl_fd(buffer, redir->pipe[1]);
		if (expansion)
			free(temp);
		free(buffer);
		buffer = readline("> ");
	}
	free(eof);
	close(redir->pipe[1]);
	if (!buffer)
		return (handle_sigint_output(), \
		mini_panic("warning", ERR_HDOC_EOF, true));
	free(buffer);
	return (true);
}

int	map_heredocs(t_msh *msh, t_list *hdocs, t_hdoc_action action)
{
	t_redircmd	*redir;

	while (hdocs)
	{
		redir = hdocs->content;
		if (action == RUN)
			run_heredoc(redir, msh);
		else if (action == OPEN_PIPES)
		{
			if (pipe(redir->pipe))
				return (false);
			redir->piped = true;
		}
		else if (action == CLOSE_PIPES_OUTPUT)
			close(redir->pipe[1]);
		else if (action == CLOSE_PIPES && redir->piped)
			close_pipe(redir->pipe);
		hdocs = hdocs->next;
	}
	return (true);
}

int	get_all_heredocs(t_cmd *ptr, void *payload)
{
	t_list **const	hdoc_list = payload;
	t_redircmd		*redir;
	t_list			*lst;
	int const		token = ptr->type;

	lst = NULL;
	if (token == EXEC)
		lst = ((t_execcmd *)ptr)->redirs;
	else if (token == SUBSHELL)
		lst = ((t_blockcmd *)ptr)->redirs;
	while (lst)
	{
		redir = lst->content;
		if (redir->redir_type == REDIR_HDOC && !lst_addback_content(hdoc_list,
				redir))
			return (false);
		lst = lst->next;
	}
	return (true);
}

int	handle_heredocs(t_cmd *root, t_msh *msh)
{
	t_list	*heredocs;
	pid_t	pid;

	heredocs = NULL;
	if (!tree_map(root, &heredocs, get_all_heredocs))
		return (free_list(heredocs), EXIT_FAILURE);
	if (!map_heredocs(msh, heredocs, OPEN_PIPES))
		map_heredocs(msh, heredocs, CLOSE_PIPES);
	pid = create_child(NULL, -1);
	if (pid == -1)
		return (EXIT_FAILURE);
	else if (pid)
	{
		map_heredocs(msh, heredocs, CLOSE_PIPES_OUTPUT);
		return (free_list(heredocs), wait_child_processes(pid));
	}
	handle_signals(EXECUTING_HDOC);
	map_heredocs(msh, heredocs, RUN);
	free_list(heredocs);
	exit(EXIT_SUCCESS);
}
