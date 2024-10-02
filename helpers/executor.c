/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/02 19:05:50 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t	execute_exec(t_execcmd *exec, char **env, int run_in_fork)
{
	t_list			*lst;		
	pid_t 			pid;
	char **const	args = get_args_arr(exec->args);
	
	if (run_in_fork)
	{
		pid = fork();
		if (pid)
			return (pid);
	}
	lst = exec->redirs;
	while (lst)
	{
		execute_redir(lst->content);
		lst = lst->next;
	}
	if (!args)
		exit(EXIT_SUCCESS);
	execute_command(args[0], args, env);
	free_string_array(args);
	exit(EXIT_FAILURE);
}

pid_t	execute_block(t_blockcmd *block, t_msh *msh, int run_in_fork)
{
	int 		status;
	t_list		*lst;
	pid_t		pid;

	if (run_in_fork)
	{
		pid = fork();
		if (pid)
			return (pid);
	}
	lst = block->redirs;
	while (lst)
	{
		execute_redir(lst->content);
		lst = lst->next;
	}
	pid = execute_cmd(block->subshell, msh, run_in_fork);
	waitpid(pid, &status, 0);
	exit(status >> 8);
}

pid_t execute_pipe(t_pipecmd *pipecmd, t_msh *msh)
{
	int 	status;
	int 	p[2];
	pid_t 	pid;
	t_list *lst;

	pid = fork();
	if (pid)
		return (pid);
	lst = pipecmd->pipelist;
	while (ft_lstlast(lst) != lst)
	{
		if (pipe(p))
			exit(1);
		pid = fork();
		if (pid == -1)
			return 0;
		if (pid)
			dup2(p[0], STDIN_FILENO);
		else
			dup2(p[1], STDOUT_FILENO);
		close_pipe(p);
		if (!pid)
			execute_cmd(lst->content, msh, 1);
		lst = lst->next;
	}
	pid = execute_cmd(lst->content, msh, 0);
	close(STDIN_FILENO);
	waitpid(pid, &status, 0);
	wait_child_processes();
	exit(status >> 8);
} 

int	execute_logic(t_opcmd *opcmd, t_msh *msh)
{
	t_cmdop const	op = opcmd->op_type;
	pid_t 			pid;
	int 			status;

	pid = execute_cmd(opcmd->left, msh, 0);
	waitpid(pid, &status, 0);
	status = status >> 8;
	if ((status && op == OP_OR) || (!status && op == OP_AND))
	{
		pid = execute_cmd(opcmd->right, msh, 0);
		if (!pid)
			return 
		waitpid(pid, &status, 0);
	}
	return (status);
}

pid_t execute_cmd(t_cmd *cmd, t_msh *msh, int forked)
{
	t_tokens const token = cmd->type;
	pid_t pid;

	pid = 0;
	if (token == EXEC)
		pid = execute_exec((t_execcmd *)cmd, msh->env, !forked);
	else if (token == PIPE)
		pid = execute_pipe((t_pipecmd *)cmd, msh);
	else if (token == SUBSHELL)
		pid = execute_block((t_blockcmd *)cmd, msh, !forked);
	else if (token == LOGIC)
		execute_logic((t_opcmd *)cmd, msh);
	return (pid);
}

void execute_hdocs(t_cmd *cmd)
{
	t_tokens const token = cmd->type;
	t_list *lst;

	if (token == EXEC || token == PIPE)
	{
		lst = NULL;
		lst = ((t_pipecmd *)cmd)->pipelist;
		if (token == EXEC)
			lst = ((t_execcmd *)cmd)->redirs;
		while (lst)
		{
			if (token == EXEC && ((t_redircmd *)lst->content)->redir_type == REDIR_HDOC)
				exec_hdoc((t_redircmd *)lst->content);
			else if (token == PIPE)
				execute_hdocs(lst->content);
			lst = lst->next;
		}
	}
	else if (token == LOGIC)
	{
		execute_hdocs(((t_opcmd *)cmd)->left);
		execute_hdocs(((t_opcmd *)cmd)->right);
	}
	else if (token == SUBSHELL)
		execute_hdocs(((t_blockcmd *)cmd)->subshell);
}

void executor(t_cmd *root, t_msh *msh)
{
	pid_t pid;

	execute_hdocs(root);
	pid = execute_cmd(root, msh, 0);
	waitpid(pid, &msh->last_status, 0);
	wait_child_processes();
}