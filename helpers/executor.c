/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/13 18:57:37 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redirects(t_list *redirs, t_msh *msh)
{
	while (redirs)
	{
		if (!execute_redir(redirs->content, msh))
			return (false);
		redirs = redirs->next;
	}
	return (true);
}

int handle_back_redirects(t_list *redirs)
{
  t_redircmd  *redir;

  while (redirs)
  {
    redir = redirs->content;
    if (dup2(redir->old_fd, redir->fd) == -1)
      return (false);
    close(redir->old_fd);
    redirs = redirs->next;
  }
  return (true);
}

/*
	STATUS:
	if no child created to execute cmd status set.
	RETURN VALUES:
	0  -> That means there is no child process to wait for.
	>0 -> A child process created. You should wait for it.
	-1 -> Error creating child process
*/
pid_t	execute_cmd(t_cmd *cmd, t_msh *msh, int *status, int pipe[2])
{
	pid_t			pid;
	int const	token = cmd->type;
	int const		builtin = get_builtin((t_execcmd *)cmd);
	int	const		should_fork = (!builtin || pipe) && token != LOGIC;
	
	if (should_fork)
	{
		pid = create_child(pipe, STDOUT_FILENO);
		if (pid)
			return (pid);
	}
	if (token == PIPE)
		*status = execute_pipe(((t_pipecmd *)cmd)->pipelist, msh);
	else if (token == SUBSHELL)
		*status = execute_block((t_blockcmd *)cmd, msh);
	else  if (token == EXEC)
		*status = execute_exec((t_execcmd *)cmd, msh, builtin);
	else if (token == LOGIC)
		*status = execute_logic((t_logiccmd *)cmd, msh);
	if (should_fork)
		exit(*status);
	return (0);
}

int	run_heredoc(t_redircmd *redir, t_msh *msh)
{
	char			*buffer;
	char			*temp;
	char			*eof;
	char			*const eof_arg = redir->args->content;
	const int		expansion = !ft_strchr(eof_arg, '\"') && 1;
	
	if (pipe(redir->pipe) == -1)
		return (mini_panic("heredoc", "pipe error", false));
	eof = unquote_arg(NULL, eof_arg);
	while (1)
	{
		buffer = readline("> ");
		if (!buffer || !ft_strncmp(buffer, eof, ft_strlen(eof) + 1))
		{
			free(buffer);
			close(redir->pipe[1]);
			free(eof);
			if (!buffer)
				return (mini_panic("heredoc", "readline error.", false));
			return (true);
		}
		temp = buffer;
		if (expansion)
			buffer = expand_dollar(buffer, ft_itoa(msh->last_status), NULL);
		write(redir->pipe[1], buffer, ft_strlen(buffer));
		write(redir->pipe[1], "\n", 1);
		if (expansion)
			free(temp);
		free(buffer);
	}
}

int	loop_heredocs(t_cmd *ptr, void *payload)
{
	t_list			*lst;
	t_redircmd		*redir;
	t_msh			*const msh = payload;
	int const	token = ((t_cmd *)ptr)->type;
	
	lst = NULL;
	if (token == EXEC)
		lst = ((t_execcmd *)ptr)->redirs;
	else if (token == SUBSHELL)
		lst = ((t_blockcmd *)ptr)->redirs;
	while (lst)
	{
		redir = lst->content;
		if (redir->redir_type == REDIR_HDOC && !run_heredoc(redir, msh))
			return (false);
		lst = lst->next;
	}
	return (true);
}

void	executor(t_cmd *root, t_msh *msh)
{
	pid_t	pid;
	if (tree_map(root, msh, loop_heredocs))
	{
		pid = execute_cmd(root, msh, &msh->last_status, NULL);
		if (pid == -1)
			mini_panic(NULL, NULL, -1);
		else if (pid != 0)
			msh->last_status = wait_child_processes(pid);
	}
}
