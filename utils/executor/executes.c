/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 09:47:20 by emyildir          #+#    #+#             */
/*   Updated: 2024/11/14 11:12:47 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_redir(t_redircmd *redir, t_msh *msh)
{
	t_redir const	type = redir->redir_type;
	char **const	args = get_args_arr(redir->args, msh);
	int				fd;
	char			*spec;

	if (!args)
		return (mini_panic(NULL, NULL, false));
	if (!*args || str_arr_size(args) > 1)
		return (mini_panic("*", "ambiguous redirect\n", -1), \
		free_string_array(args), false);
	spec = args[0];
	if (type == REDIR_HDOC)
		fd = redir->pipe[0];
	else
		fd = open(spec, get_redir_flags(type), S_IRWXU);
	if (fd == -1)
		return (mini_panic(spec, NULL, -1), \
		free_string_array(args), false);
	redir->old_fd = dup(redir->fd);
	if ((redir->old_fd == -1 && errno != EBADF)
		|| (dup2(fd, redir->fd) == -1))
		return (mini_panic(spec, NULL, -1), \
		free_string_array(args), false);
	return (close(fd), free_string_array(args), true);
}

int	execute_exec(t_execcmd *exec, t_msh *msh, int builtin)
{
	char **const	args = get_args_arr(exec->args, msh);
	int				status;

	if (!handle_redirects(exec->redirs, msh))
	{
		handle_back_redirects(exec->redirs);
		return (EXIT_FAILURE);
	}
	status = EXIT_SUCCESS;
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
	free_string_array(args);
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

	handle_signals(NOTHING);
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
		waitpid(pid, &status, 0);
	if (!WIFEXITED(status))
	{
		handle_sigint_output();
		return (get_status(status));
	}
	status = get_status(status);
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
