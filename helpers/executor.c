/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/05 09:16:25 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(t_list *argslist)
{
	char	**const args = get_args_arr(argslist);
	
	if (!argslist)
		return (false);
	if (!ft_strncmp(args[0], "cd", 3)
		|| !ft_strncmp(args[0], "exit", 5)
		|| !ft_strncmp(args[0], "pwd", 4)
		|| !ft_strncmp(args[0], "echo", 5)
		|| !ft_strncmp(args[0], "export", 7)
		|| !ft_strncmp(args[0], "unset", 6)
		|| !ft_strncmp(args[0], "env", 4))
		return (true);
	return (false);
}

int	execute_builtin(char **args, t_msh *msh)
{
	int		args_size;
	int		status;
	(void)msh;

	args_size = str_arr_size(args);
	status = EXIT_FAILURE;
	if (!ft_strncmp(args[0], "cd", 3))
		status = builtin_cd(args_size, args, msh);
	else if (!ft_strncmp(args[0], "exit", 5))
		status = builtin_exit(args_size, args, msh);
	else if (!ft_strncmp(args[0], "pwd", 4))
		status = builtin_pwd(args_size, args, msh);
	else if (!ft_strncmp(args[0], "echo", 5))
		status = builtin_echo(args_size, args, msh);
	else if (!ft_strncmp(args[0], "export", 7))
		status = builtin_export(args_size, args, msh);
	else if (!ft_strncmp(args[0], "unset", 6))
		status = builtin_unset(args_size, args, msh);
	else if (!ft_strncmp(args[0], "env", 4))
		status = builtin_env(args_size, args, msh);
	return (status);
}

int	execute_cmd(t_cmd *cmd, t_msh *msh, int should_fork)
{
	int				builtin;
	pid_t			pid;
	t_tokens const	token = cmd->type;
	
	builtin = token == EXEC && is_builtin(((t_execcmd *)cmd)->args);
	if ((should_fork && !builtin)
		&& (token == EXEC || token == PIPE || token == SUBSHELL))
	{
		pid = fork();
		if (pid == -1)
			return (mini_panic(NULL, NULL, false, -1), EXIT_FAILURE);
		if (pid)
			return (wait_child_processes(pid));
	}
	if (token == EXEC)
		execute_exec((t_execcmd *)cmd, msh, builtin);
	else if (token == PIPE)
		execute_pipe((t_pipecmd *)cmd, msh);
	else if (token == SUBSHELL)
		execute_block((t_blockcmd *)cmd, msh);
	else if (token == LOGIC)
		return (execute_logic((t_logiccmd *)cmd, msh));
	return (EXIT_FAILURE);
}

int	run_heredoc(t_redircmd *redir)
{
	char			*buffer;
	const size_t	len = redir->e_spec - redir->s_spec;

	if (pipe(redir->pipe) == -1)
		return (mini_panic(NULL, NULL, false, -1), false);
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
				mini_panic(NULL, "readline error.", false, false);
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
	int				status;

	status = true;
	lst = NULL;
	if (token == EXEC)
		lst = ((t_execcmd *)cmd)->redirs;
	else if (token == PIPE)
		lst = ((t_pipecmd *)cmd)->pipelist;
	else if (token == LOGIC)
	{
		loop_heredocs(((t_logiccmd *)cmd)->left);
		loop_heredocs(((t_logiccmd *)cmd)->right);
	}
	else if (token == SUBSHELL)
	{
		loop_heredocs(((t_blockcmd *)cmd)->subshell);
		lst = ((t_blockcmd *)cmd)->redirs;
	}
	while (lst)
	{
		if ((token == EXEC || token == SUBSHELL)
			&& ((t_redircmd *)lst->content)->redir_type == REDIR_HDOC)
		{
			status = run_heredoc((t_redircmd *)lst->content);
			if (!status)
				break;
		}
		else if (token == PIPE)
			loop_heredocs(lst->content);
		lst = lst->next;
	}
	if (!status)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	executor(t_cmd *root, t_msh *msh)
{
	int		status;

	status = loop_heredocs(root);
	if (!status)
		msh->last_status = execute_cmd(root, msh, true);
}
