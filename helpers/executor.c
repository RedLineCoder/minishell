/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/13 18:40:03 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		execute_redir(t_redircmd *redir)
{
	int		fd;
	char	*file;
	int		flags;
	
	flags = O_RDWR | O_CREAT;
	if (redir->redir_type == REDIR_APPEND)
		flags |= O_APPEND; 
	file = ft_strndup(redir->s_spec, redir->e_spec - redir->s_spec);
	fd = open(file, flags, S_IRWXU);
	free(file);
	if (fd == -1)
		return (0);
	dup2(fd, redir->fd);
	return (1);
}

int		execute_execcmd(t_execcmd *exec, char **env, int *input, int output)
{
	pid_t	pid;
	int		status;
	t_list	*lst;
	t_redircmd	*redir;
	char	**const args = get_args_arr(exec->args);
	
	pid = fork();
	if (pid == -1)
		return (free_string_array(args), 0);
	else if (!pid)
	{
		if (input)
			dup_io(input[0], STDOUT_FILENO, input[1]);
		if (output)
		{
			dup2(exec->fd[1], STDOUT_FILENO);
			close(exec->fd[0]);
		}
		lst = exec->redirs;
		while (lst)
		{
			redir = lst->content;
			if (redir->redir_type != REDIR_HDOC)
				execute_redir(lst->content);
			lst = lst->next;
		}
		if (args)
			execute_command(args[0], args, env);
	}
	waitpid(pid, &status, 0);
	free_string_array(args);
	return (status);
}
int		execute_block(t_binode	*block, t_msh *msh, int *fd)
{
	int			status;
	int			left_token;
	int			right_token;

	if (!block)
		return (0);
		
	(void)fd;
	status = 0;
	left_token = TKN_NONE;
	right_token = TKN_NONE;
	if (block->right)
		right_token = ((t_opcmd *)block->right->cmd)->op_type;
	if (block->left)
		left_token = ((t_cmd *)block->left->cmd)->type;
	if (left_token == BLOCK)
		status = executor((t_binode *)block->left, msh);
	else if (left_token == EXEC)
	{
		status = execute_execcmd((t_execcmd *)block->left->cmd, msh->env, NULL, 0);
		if (right_token == OP_PIPE)
			return (execute_block((t_binode *)block->right, msh, NULL));
	}
	block = (t_binode *)block->right;
	if ((!status && right_token == OP_OR) 
		|| (status && right_token == OP_AND))
	{
		block = (t_binode *)block->right;
		while (block && ((t_opcmd *)block->cmd)->op_type == OP_PIPE)
			block = (t_binode *)block->right;
	}
	return (executor((t_binode *)block, msh)); 
}


int	executor(t_binode *root, t_msh *msh)
{
	pid_t	pid;
	int		status;
	
	if (!root)
		return (0);
	pid = fork();
	if (pid == -1)
		return (0);
	else if (pid == 0)
	{
		execute_block(root, msh, NULL);
		exit(0);
	}
	waitpid(pid, &status, 0);
	return (status);	
}