/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 20:47:31 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/24 21:14:17 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	close_pipes_output_end(t_list	*hdocs)
{
	t_redircmd	*redir;
	
	while (hdocs)
	{
		redir = hdocs->content;
		close(redir->pipe[1]);
		hdocs = hdocs->next;
	}
	return (true);
}

int	open_pipes(t_list *hdocs)
{
	t_redircmd	*redir;
	
	while (hdocs)
	{
		redir = hdocs->content;
		if(pipe(redir->pipe))
			return (false);
		hdocs = hdocs->next;
	}
	return (true);
}

int     set_all_heredocs(t_cmd *ptr, void *payload)
{
    t_redircmd      *redir;
    t_list          *lst;
    t_list          **const hdoc_list = payload;
    int const		token = ((t_cmd *)ptr)->type;

    lst = NULL;
    if (token == EXEC)
		lst = ((t_execcmd *)ptr)->redirs;
	else if (token == SUBSHELL)
		lst = ((t_blockcmd *)ptr)->redirs;
	while (lst)
	{
		redir = lst->content;
		if (redir->redir_type == REDIR_HDOC 
            && !lst_addback_content(hdoc_list, redir))
			return (false);
		lst = lst->next;
	}
    return (true);
}

int	run_heredocs(t_list	*hdocs, t_msh *msh)
{
	t_redircmd		*redir;

	while (hdocs)
	{
		redir = hdocs->content;
		if (!run_heredoc(redir, msh))
			return (false);
		hdocs = hdocs->next;
	}
	return (true);
}

int		handle_heredocs(t_cmd *root, t_msh *msh)
{
	pid_t	pid;
	t_list  *heredocs;
    
	heredocs = NULL;
	job = EXECUTING_HDOC;
    if (!tree_map(root, &heredocs, set_all_heredocs))
		return (free_list(heredocs), EXIT_FAILURE);
	open_pipes(heredocs);
	pid = create_child(NULL, -1);
	if (pid == -1)
		return (EXIT_FAILURE);
	else if (pid)
	{
		close_pipes_output_end(heredocs);
		return (wait_child_processes(pid));
	}
	if (!run_heredocs(heredocs, msh))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}