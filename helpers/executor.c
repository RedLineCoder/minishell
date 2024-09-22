/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/22 18:59:49 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t	execute_exec(t_execcmd *exec, char **env, int piped)
{
	const pid_t		pid = fork();
	char **const	args = get_args_arr(exec->args);

	if (pid == -1)
		return (free_string_array(args), 0);
	if (!pid)
	{
		if (piped)
			close(exec->fd[0]);
		while (exec->redirs)
		{
			if (((t_redircmd *)exec->redirs->content)->redir_type != REDIR_HDOC)
				execute_redir(exec, exec->redirs->content);
			exec->redirs = exec->redirs->next;
		}
		if (exec->in_file != -1)
			dup2(exec->in_file, STDIN_FILENO);
		if(exec->out_file != -1)
			dup2(exec->out_file, STDOUT_FILENO);
		if (args)
			execute_command(args[0], args, env);
	}
	if (!piped)
		close_pipe(exec->fd);
	return (free_string_array(args), pid);
}

int	execute_node(t_node *node, t_msh *msh, pid_t *last)
{
	const int		is_piped = get_node_type(node->right) == OP_PIPE;
	const t_tokens	l_token = get_node_type(node->left);
	t_cmd *const	left = get_node_cmd(node->left);

	if (l_token == BLOCK)
		return (execute_block(node->left, msh, 0));
	else if (l_token == EXEC)
	{
		if(is_piped)
			exec_pipe((t_execcmd *)left);
		*last = execute_exec((t_execcmd *)left, msh->env, \
		is_piped);
	}  
	if (is_piped && node->right->left)
	{
		close(((t_execcmd *)left)->fd[1]);
		((t_execcmd *)node->right->left->cmd)->in_file = ((t_execcmd *)left)->fd[0];
		return (execute_node(node->right, msh, last));
	}
	waitpid(*last, &msh->last_status, 0);
	wait_child_processes();
	node = get_next_block(node->right, msh->last_status);
	if (node)
		return (execute_block(node, msh, 0));
	return (1);
}

void	execute_hdocs(t_node *block)
{
	t_list		*lst;
	t_redircmd	*redir;
	t_execcmd	*exec;
	
	while (block)
	{
		exec = (t_execcmd *)get_node_cmd(block->left);
		if (exec && exec->type == EXEC)
		{
			lst = exec->redirs;
			while (lst)
			{
				redir = lst->content;
				if (redir->redir_type == REDIR_HDOC)
					exec_hdoc(exec, redir);
				lst = lst->next;
			}
		}
		block = block->right;
	}
}

int	execute_block(t_node *block, t_msh *msh, int is_root)
{
	pid_t	pid;
	pid_t	last;
	
	if (!block)
		return (0);
	if (!is_root)
	{
		pid = fork();
		if (pid == -1)
			return (0);
		else if (pid == 0)
		{
			execute_node(block, msh, &last);
			exit(msh->last_status);
		}
		while (block)
		{
			if (block->left && block->left->cmd->type == EXEC)
				close_pipe(((t_execcmd *)block->left->cmd)->fd);
			block = block->right;
		}
		waitpid(pid, NULL, 0);
	}
	else
		execute_node(block, msh, &last);
	return (msh->last_status);
}

void	executor(t_node *block, t_msh *msh)
{
	execute_hdocs(block);
	execute_block(block, msh, 1);
}