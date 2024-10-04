/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 09:47:20 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/04 13:50:51 by emyildir         ###   ########.fr       */
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
		return (dup2(redir->pipe[0], redir->fd), \
		close(redir->pipe[0]), true);
	flags = O_RDWR;
	if (type == REDIR_APPEND)
		flags |= O_APPEND;
	if (type != REDIR_INPUT)
		flags |= O_CREAT;
	file = ft_strndup(redir->s_spec, redir->e_spec - redir->s_spec);
	fd = open(file, flags, S_IRWXU);
	free(file);
	if (fd == -1)
		return (false);
	if (type == REDIR_APPEND || type == REDIR_OUTPUT)
		dup2(fd, redir->fd);
	else
		dup2(fd, redir->fd);
	close(fd);
	return (true);
}

void	execute_exec(t_execcmd *exec, char **env)
{
	t_list			*lst;		
	char **const	args = get_args_arr(exec->args);

	if (!args)
		mini_panic("Malloc error\n", true, EXIT_FAILURE);
	lst = exec->redirs;
	while (lst)
	{
		if (!execute_redir(lst->content))
		{
			free(args);
			exit(EXIT_FAILURE);
		}
		lst = lst->next;
	}
	if (!*args)
		exit(EXIT_SUCCESS);
	execute_command(args[0], args, env);
	free_string_array(args);
	exit(EXIT_FAILURE);
}

void	execute_block(t_blockcmd *block, t_msh *msh)
{
	int		status;
	t_list	*lst;

	lst = block->redirs;
	while (lst)
	{
		execute_redir(lst->content);
		lst = lst->next;
	}
	status = execute_cmd(block->subshell, msh, true);
	exit(status);
}

void	execute_pipe(t_pipecmd *pipecmd, t_msh *msh)
{
	int		p[2];
	pid_t	pid;
	t_list	*lst;

	lst = pipecmd->pipelist;
	while (lst)
	{
		if (pipe(p))
			exit(1);
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid)
			dup2(p[0], STDIN_FILENO);
		else if (ft_lstlast(lst) != lst)
			dup2(p[1], STDOUT_FILENO);
		close_pipe(p);
		if (!pid)
			execute_cmd(lst->content, msh, false);
		lst = lst->next;
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
