/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 07:59:05 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/23 10:35:58 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(int builtin, char **args, t_msh *msh)
{
	int const	args_size = str_arr_size(args);
	int			(*f[9])(int, char **, t_msh *);

	f[BUILTIN_NONE] = NULL;
	f[BUILTIN_ECHO] = builtin_echo;
	f[BUILTIN_CD] = builtin_cd;
	f[BUILTIN_PWD] = builtin_pwd;
	f[BUILTIN_EXPORT] = builtin_export;
	f[BUILTIN_UNSET] = builtin_unset;
	f[BUILTIN_ENV] = builtin_env;
	f[BUILTIN_EXIT] = builtin_exit;
	f[BUILTIN_STATUS] = builtin_status;
	return (f[builtin](args_size, args, msh));
}

int	get_builtin(t_execcmd *exec)
{
	int			i;
	char		*cmd;
	char		*cmds[9];

	if (exec->type != EXEC || !(exec->args && exec->args->content))
		return (false);
	cmd = exec->args->content;
	cmds[BUILTIN_ECHO] = "echo";
	cmds[BUILTIN_CD] = "cd";
	cmds[BUILTIN_PWD] = "pwd";
	cmds[BUILTIN_EXPORT] = "export";
	cmds[BUILTIN_UNSET] = "unset";
	cmds[BUILTIN_ENV] = "env";
	cmds[BUILTIN_EXIT] = "exit";
	cmds[BUILTIN_STATUS] = "status";
	i = 0;
	while (++i < 9)
		if (!ft_strncmp(cmds[i], cmd, ft_strlen(cmd) + 1))
			return (i);
	return (BUILTIN_NONE);
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
	pid_t		pid;
	int const	token = cmd->type;
	int const	builtin = get_builtin((t_execcmd *)cmd);
	int const	should_fork = (!builtin || pipe) && token != LOGIC;

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
	else if (token == EXEC)
		*status = execute_exec((t_execcmd *)cmd, msh, builtin);
	else if (token == LOGIC)
		*status = execute_logic((t_logiccmd *)cmd, msh);
	if (should_fork)
		exit(*status);
	return (0);
}

int	loop_heredocs(t_cmd *ptr, void *payload)
{
	t_list			*lst;
	t_redircmd		*redir;
	t_msh *const	msh = payload;
	int const		token = ((t_cmd *)ptr)->type;

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

	job = EXECUTING_HDOC;
	if (tree_map(root, msh, loop_heredocs))
	{
		job = WAITING_CMD;
		pid = execute_cmd(root, msh, &msh->last_status, NULL);
		if (pid == -1)
			msh->last_status = mini_panic(NULL, NULL, -1);
		else if (pid != 0)
			msh->last_status = wait_child_processes(pid);
	}
	else
		msh->last_status = EXIT_FAILURE;
}
