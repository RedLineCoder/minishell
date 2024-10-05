/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 09:47:20 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/05 18:56:25 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_redir(t_redircmd *redir)
{
	int				fd;
	char			*file;
	int				flags;
	t_redir const	type = redir->redir_type;

	if (type == REDIR_HDOC)
		fd = redir->pipe[0];
	flags = 0;
	if (type == REDIR_INPUT)
		flags |= O_RDONLY;
	if (type == REDIR_OUTPUT)
		flags |= O_WRONLY;
	if (type == REDIR_APPEND)
		flags |= O_APPEND;
	if (type != REDIR_INPUT)
		flags |= O_CREAT;
	file = ft_strndup(redir->s_spec, redir->e_spec - redir->s_spec);
	if (type != REDIR_HDOC)
		fd = open(file, flags, S_IRWXU);
	if (fd == -1 || dup2(fd, redir->fd) == -1)
		return (mini_panic(file, NULL, false, -1), free(file), false);
	close(fd);
	free(file);
	return (true);
}

int	execute_exec(t_execcmd *exec, t_msh *msh, int builtin)
{
	int			status;
	char		**args;

	if (loop_redirects(exec->redirs))
		return (mini_panic("exec", "malloc error\n", !builtin, EXIT_FAILURE));
	args = get_args_arr(exec->args);
	if (!args)
		return (mini_panic("exec", "malloc error\n", !builtin, EXIT_FAILURE));
	if (!*args)
	{
		free_string_array(args);
		if (builtin)
			return (EXIT_SUCCESS);
		exit(EXIT_FAILURE);
	}
	if (builtin)
		status = execute_builtin(builtin, args, msh);
	else
		execute_command(args[0], args, msh->env, false);
	free_string_array(args);
	if (builtin)
		return (status);
	exit(EXIT_FAILURE);
}

void	execute_block(t_blockcmd *block, t_msh *msh)
{
	int		status;
	
	if (loop_redirects(block->redirs))
		mini_panic("exec", "malloc error\n", true, EXIT_FAILURE);
	status = execute_cmd(block->subshell, msh, true);
	exit(status);
}

void	execute_pipe(t_list *pipelist, t_msh *msh)
{
	int		p[2];
	pid_t	pid;

	while (pipelist)
	{
		if (pipe(p))
			mini_panic(NULL, NULL, true, EXIT_FAILURE);
		pid = fork();
		if (pid == -1)
		{
			close_pipe(p);
			mini_panic(NULL, NULL, true, EXIT_FAILURE);
		}
		if (pid)
			dup2(p[0], STDIN_FILENO);
		else if (ft_lstlast(pipelist) != pipelist)
			dup2(p[1], STDOUT_FILENO);
		close_pipe(p);
		if (!pid)
			execute_cmd(pipelist->content, msh, false);
		pipelist = pipelist->next;
	}
	close(STDIN_FILENO);
	exit(wait_child_processes(pid));
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
