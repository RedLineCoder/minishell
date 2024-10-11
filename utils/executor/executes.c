/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 09:47:20 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/11 14:42:30 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_redir(t_redircmd *redir, t_msh *msh)
{
	int				fd;
	char			*file;
	int				flags;
	t_redir const	type = redir->redir_type;

	flags = 0;
	if (type == REDIR_INPUT)
		flags |= O_RDONLY;
	if (type == REDIR_OUTPUT)
		flags |= O_WRONLY;
	if (type == REDIR_APPEND)
		flags |= O_APPEND;
	if (type != REDIR_INPUT)
		flags |= O_CREAT;
	redir->args = expander(redir->args, msh);
	if (ft_lstsize(redir->args) > 1)
		return (mini_panic("*", "ambiguous redirect", false));
	file = redir->args->content;
	if (type == REDIR_HDOC)
		fd = redir->pipe[0];
	else
		fd = open(file, flags, S_IRWXU);
	if (fd == -1 || dup2(fd, redir->fd) == -1)
		return (mini_panic(file, NULL, false));
	return (close(fd), true);
}

int	execute_exec(t_execcmd *exec, t_msh *msh, int builtin)
{
	int			status;
	char		**args;

	if (!handle_redirects(exec->redirs, msh))
		return (EXIT_FAILURE);
	args = get_args_arr(exec->args);
	if (!args)
		return (mini_panic(NULL, "malloc error\n", EXIT_FAILURE));
	else if (*args)
	{
		if (builtin)
			status = execute_builtin(builtin, args, msh);
		else
		{
			execute_command(args[0], args, msh->env, false);
			status = EXIT_FAILURE;
		}
	}
	free(args);
	return (status);
}

int	execute_block(t_blockcmd *block, t_msh *msh)
{
	int		status;

	if (!handle_redirects(block->redirs, msh))
		return (mini_panic("block", "malloc error\n", EXIT_FAILURE));
	status = execute_cmd(block->subshell, msh, true);
	return (status);
}

int	execute_pipe(t_list *pipelist, t_msh *msh)
{
	int		last;
	int		p[2];
	pid_t	pid;

	while (pipelist)
	{
		last = ft_lstlast(pipelist) == pipelist;
		if (!last && pipe(p))
			return (mini_panic(NULL, NULL, EXIT_FAILURE));
		pid = fork();
		if (pid == -1
			|| (!last && ((pid && dup2(p[0], STDIN_FILENO) == -1)
					|| (!pid && dup2(p[1], STDOUT_FILENO) == -1))))
		{
			if (!last)
				close_pipe(p);
			return (mini_panic(NULL, NULL, EXIT_FAILURE));
		}
		close_pipe(p);
		if (!pid)
			exit(execute_cmd(pipelist->content, msh, false));
		pipelist = pipelist->next;
	}
	close(STDIN_FILENO);
	return (wait_child_processes(pid));
}

int	execute_logic(t_logiccmd *logiccmd, t_msh *msh)
{
	t_logicop const	op = logiccmd->op_type;
	int				status;

	status = execute_cmd(logiccmd->left, msh, true);
	if ((status && op == OP_OR) || (!status && op == OP_AND))
		status = execute_cmd(logiccmd->right, msh, true);
	return (status);
}
