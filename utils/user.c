/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:20:28 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/13 19:03:44 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Was working but need to rewrite this due errors causing by PATH variable.
char	*get_user(t_list *env)
{
	(void)env;
	return (ft_strdup("emyildir"));
	/*
	int		p[2];
	pid_t	pid;
	char	*user;
	char	**args;

	args = ft_split("whoami", ' ');
	if (!args)
		return (NULL);
	if (pipe(p) < 0)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (free(args), close(p[0]), close(p[1]), NULL);
	else if (!pid && dup2(p[1], STDOUT_FILENO) == STDOUT_FILENO && !close(p[0]))
		execute_command("whoami", args, env);
	waitpid(pid, NULL, 0);
	close(p[1]);
	user = get_next_line(p[0]);
	close(p[0]);
	if (!user)
		return (free_string_array(args), NULL);
	user[ft_strlen(user) - 1] = '\0';
	return (free_string_array(args), user);
	*/
}
