/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/10 19:50:19 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redirects(t_list *redirs)
{
	t_redircmd	*redir;

	while (redirs)
	{
		redir = redirs->content;
		if (!execute_redir(redirs->content))
			return (false);
		redirs = redirs->next;
	}
	return (true);
}

int	execute_cmd(t_cmd *cmd, t_msh *msh, int should_fork)
{
	int				status;
	pid_t			pid;
	t_tokens const	token = cmd->type;
	int const		builtin = get_builtin((t_execcmd *)cmd);

	should_fork = should_fork && !builtin
		&& (token == EXEC || token == PIPE || token == SUBSHELL);
	if (should_fork)
	{
		pid = fork();
		if (pid == -1)
			return (mini_panic(NULL, NULL, EXIT_FAILURE));
		if (pid)
			return (wait_child_processes(pid));
	}
	if (token == PIPE)
		status = execute_pipe(((t_pipecmd *)cmd)->pipelist, msh);
	else if (token == SUBSHELL)
		status = execute_block((t_blockcmd *)cmd, msh);
	else if (token == EXEC)
		status = execute_exec((t_execcmd *)cmd, msh, builtin);
	else if (token == LOGIC)
		return (execute_logic((t_logiccmd *)cmd, msh));
	if (should_fork)
		exit(status);
	return (status);
}

int	run_heredoc(t_redircmd *redir)
{
	char			*buffer;
	const size_t	len = redir->e_spec - redir->s_spec;

	if (pipe(redir->pipe) == -1)
		return (mini_panic("heredoc", "pipe error", false));
	while (1)
	{
		buffer = readline("> ");
		if (!buffer || (ft_strlen(buffer) == len
				&& !ft_strncmp(buffer, redir->s_spec, len)))
		{
			free(buffer);
			close(redir->pipe[1]);
			if (!buffer)
				return (mini_panic("heredoc", "readline error.", false));
			return (true);
		}
		write(redir->pipe[1], buffer, ft_strlen(buffer));
		write(redir->pipe[1], "\n", 1);
		free(buffer);
	}
}

int	loop_heredocs(void *ptr)
{
	t_list			*lst;
	t_redircmd		*redir;
	t_tokens const	token = ((t_cmd *)ptr)->type;
	
	lst = NULL;
	if (token == EXEC)
		lst = ((t_execcmd *)ptr)->redirs;
	else if (token == SUBSHELL)
		lst = ((t_blockcmd *)ptr)->redirs;
	while (lst)
	{
		redir = lst->content;
		if (redir->redir_type == REDIR_HDOC && !run_heredoc(redir))
			return (false);
		lst = lst->next;
	}
	return (true);
}

void	executor(t_cmd *root, t_msh *msh)
{
	if (tree_map(root, loop_heredocs))
		msh->last_status = execute_cmd(root, msh, true);
}
