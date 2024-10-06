/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 15:51:29 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/06 20:00:26 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_builtin(t_execcmd *exec)
{
	int			i;
	char		*cmd;
	char		*cmds[8];

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
	i = 0;
	while (++i < 8)
		if (!ft_strncmp(cmds[i], cmd, ft_strlen(cmd) + 1))
			return (i);
	return (BUILTIN_NONE);
}

int	execute_builtin(int builtin, char **args, t_msh *msh)
{
	int const	args_size = str_arr_size(args);
	int			(*f[8])(int, char **, t_msh *);

	f[BUILTIN_NONE] = NULL;
	f[BUILTIN_ECHO] = builtin_echo;
	f[BUILTIN_CD] = builtin_cd;
	f[BUILTIN_PWD] = builtin_pwd;
	f[BUILTIN_EXPORT] = builtin_export;
	f[BUILTIN_UNSET] = builtin_unset;
	f[BUILTIN_ENV] = builtin_env;
	f[BUILTIN_EXIT] = builtin_exit;
	return (f[builtin](args_size, args, msh));
}
