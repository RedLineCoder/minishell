/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/03 17:47:38 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_cmd(t_cmd *cmd, t_msh *msh, int should_fork)
{
	t_tokens const	token = cmd->type;
	pid_t			pid;

	if (should_fork
		&& (token == EXEC || token == PIPE || token == SUBSHELL))
	{
		pid = fork();
		if (pid == -1)
			return (EXIT_FAILURE);
		if (pid)
			return (wait_child_processes(pid));
	}
	if (token == EXEC)
		execute_exec((t_execcmd *)cmd, msh->env);
	else if (token == PIPE)
		execute_pipe((t_pipecmd *)cmd, msh);
	else if (token == SUBSHELL)
		execute_block((t_blockcmd *)cmd, msh);
	else if (token == LOGIC)
		return (execute_logic((t_opcmd *)cmd, msh));
	return (EXIT_FAILURE);
}

int	run_heredoc(t_redircmd *redir)
{
	char			*buffer;
	const size_t	len = redir->e_spec - redir->s_spec;

	if (pipe(redir->pipe) == -1)
		return (false);
	while (1)
	{
		buffer = readline("> ");
		if (!buffer || (ft_strlen(buffer) == len
				&& !ft_strncmp(buffer, redir->s_spec, len)))
		{
			free(buffer);
			close(redir->pipe[1]);
			if (!buffer)
			{
				mini_panic("Heredoc error.", false, false);
				return (false);
			}
			return (true);
		}
		write(redir->pipe[1], buffer, ft_strlen(buffer));
		write(redir->pipe[1], "\n", 1);
		free(buffer);
	}
}

int	loop_heredocs(t_cmd *cmd)
{
	t_tokens const	token = cmd->type;
	t_list			*lst;

	lst = NULL;
	if (token == EXEC)
		lst = ((t_execcmd *)cmd)->redirs;
	else if (token == PIPE)
		lst = ((t_pipecmd *)cmd)->pipelist;
	else if (token == LOGIC)
	{
		loop_heredocs(((t_opcmd *)cmd)->left);
		loop_heredocs(((t_opcmd *)cmd)->right);
	}
	else if (token == SUBSHELL)
		loop_heredocs(((t_blockcmd *)cmd)->subshell);
	while (lst)
	{
		if (token == EXEC
			&& ((t_redircmd *)lst->content)->redir_type == REDIR_HDOC)
			run_heredoc((t_redircmd *)lst->content);
		else if (token == PIPE)
			loop_heredocs(lst->content);
		lst = lst->next;
	}
	return (EXIT_SUCCESS);
}

void	executor(t_cmd *root, t_msh *msh)
{
	int		status;

	status = loop_heredocs(root);
	if (!status)
		msh->last_status = execute_cmd(root, msh, true);
}
