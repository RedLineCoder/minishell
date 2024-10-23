/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 09:47:20 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/23 09:38:36 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_redir(t_redircmd *redir, t_msh *msh)
{
	int				fd;
	char			*spec;
	t_redir const	type = redir->redir_type;

	redir->args = expander(redir->args, msh);
	if (ft_lstsize(redir->args) > 1)
		return (mini_panic("*", "ambiguous redirect\n", false));
	spec = redir->args->content;
	if (type == REDIR_HDOC)
		fd = redir->pipe[0];
	else
		fd = open(spec, get_redir_flags(type), S_IRWXU);
	if (fd == -1)
		return (mini_panic(spec, NULL, false));
	redir->old_fd = dup(redir->fd);
	if (redir->old_fd == -1 && errno != EBADF)
		return (mini_panic(spec, NULL, false));
	if (dup2(fd, redir->fd) == -1)
		return (mini_panic(spec, NULL, false));
	return (close(fd), true);
}

int	execute_exec(t_execcmd *exec, t_msh *msh, int builtin)
{
	int			status;
	char		**args;

	if (!handle_redirects(exec->redirs, msh))
	{
		handle_back_redirects(exec->redirs);
		return (EXIT_FAILURE);
	}
	status = EXIT_SUCCESS;
	exec->args = expander(exec->args, msh);
	args = get_args_arr(exec->args);
	if (!args)
		return (mini_panic(NULL, "malloc error\n", EXIT_FAILURE));
	else if (*args)
	{
		if (builtin)
			status = execute_builtin(builtin, args, msh);
		else
			status = run_command(args[0], args, msh->env);
	}
	if (!handle_back_redirects(exec->redirs))
		return (free(args), mini_panic(NULL, NULL, EXIT_FAILURE));
	free(args);
	return (status);
}

int	execute_block(t_blockcmd *block, t_msh *msh)
{
	int		status;
	pid_t	pid;

	if (!handle_redirects(block->redirs, msh))
		return (mini_panic("block", "malloc error\n", EXIT_FAILURE));
	pid = execute_cmd(block->subshell, msh, &status, NULL);
	if (pid == -1)
		return (mini_panic(NULL, NULL, EXIT_FAILURE));
	if (pid)
		status = wait_child_processes(pid);
	return (status);
}

int	execute_pipe(t_list *pipelist, t_msh *msh)
{
	int		last;
	int		p[2];
	int		status;
	pid_t	pid;
	
	job = EXECUTING_PIPE;
	while (pipelist)
	{
		last = ft_lstlast(pipelist) == pipelist;
		if (!last && pipe(p))
			return (mini_panic(NULL, NULL, EXIT_FAILURE));
		pid = execute_cmd(pipelist->content, msh, &status, p);
		if (pid == -1 || (!last && dup2(p[0], STDIN_FILENO) == -1))
		{
			if (!last)
				close_pipe(p);
			return (mini_panic(NULL, NULL, EXIT_FAILURE));
		}
		close_pipe(p);
		pipelist = pipelist->next;
	}
	close(STDIN_FILENO);
	if (!pid)
		return (status);
	return (wait_child_processes(pid));
}

int	execute_logic(t_logiccmd *logiccmd, t_msh *msh)
{
	t_logicop const	op = logiccmd->op_type;
	int				status;
	pid_t			pid;

	pid = execute_cmd(logiccmd->left, msh, &status, NULL);
	if (pid == -1)
		return (mini_panic(NULL, NULL, EXIT_FAILURE));
	if (pid)
		status = wait_child_processes(pid);
	if ((status && op == OP_OR) || (!status && op == OP_AND))
	{
		pid = execute_cmd(logiccmd->right, msh, &status, NULL);
		if (pid == -1)
			return (mini_panic(NULL, NULL, EXIT_FAILURE));
		if (pid)
			status = wait_child_processes(pid);
	}
	return (status);
}
