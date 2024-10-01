/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/01 18:31:46 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int execute_exec(t_execcmd *exec, char **env)
{
	t_list			*lst;		
	char **const	args = get_args_arr(exec->args);
	pid_t const		pid = fork();
	
	if (pid)
		return (pid);
	if (!args)
		exit(1);
	lst = exec->redirs;
	while (lst)
	{
		execute_redir(lst->content);
		lst = lst->next;
	}
	if (args)
		execute_command(args[0], args, env);
	return (free_string_array(args), exit(1), 0);
}

pid_t	execute_block(t_blockcmd *block, t_msh *msh)
{
	int 		status;
	t_list		*lst;
	pid_t		pid;

	pid = fork();
	if (pid)
		return pid;
	lst = block->redirs;
	while (lst)
	{
		execute_redir(lst->content);
		lst = lst->next;
	}
	pid = execute_cmd(block->subshell, msh);
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
	if (pipe(p))
		exit(1);
	lst = pipecmd->pipelist;
	while (lst)
	{
		if (ft_lstsize(lst) > 1)
		{
			pid = fork();
			if (pid == -1)
				return 0;
			if (!pid)
			{
				dup2(p[0], STDIN_FILENO);
				close_pipe(p);
				if (pipe(p))
					exit(1);
			}
			else
			{
				dup2(p[1], STDOUT_FILENO);
				close_pipe(p);
				pid = execute_cmd(lst->content, msh);
				waitpid(pid, &status, 0);
				exit(status >> 8);
			}
		}
		else
			pid = execute_cmd(lst->content, msh);
		lst = lst->next;
	}
	waitpid(pid, &status, 0);
	wait_child_processes();
	exit(status >> 8);
}

int	execute_logic(t_opcmd *opcmd, t_msh *msh)
{
	t_cmdop const	op = opcmd->op_type;
	pid_t 			pid;
	int 			status;

	pid = execute_cmd(opcmd->left, msh);
	waitpid(pid, &status, 0);
	status = status >> 8;
	if ((status && op == OP_OR) || (!status && op == OP_AND))
	{
		pid = execute_cmd(opcmd->right, msh);
		waitpid(pid, &status, 0);
	}
	return (status);
}

pid_t execute_cmd(t_cmd *cmd, t_msh *msh)
{
	t_tokens const token = cmd->type;
	pid_t pid;

	pid = 0;
	if (token == EXEC)
		pid = execute_exec((t_execcmd *)cmd, msh->env);
	else if (token == PIPE)
		pid = execute_pipe((t_pipecmd *)cmd, msh);
	else if (token == SUBSHELL)
		pid = execute_block((t_blockcmd *)cmd, msh);
	else if (token == LOGIC)
		pid = execute_logic((t_opcmd *)cmd, msh);
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
	pid = execute_cmd(root, msh);
	waitpid(pid, &msh->last_status, 0);
}