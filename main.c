/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/08/26 05:10:58 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_panic(void)
{
	exit(1);
}

char	*get_prompt(t_msh *msh)
{
	char	path[PATH_MAX];
	char	*buffer;

	buffer = ft_strjoin(msh->user, " ~ ");
	if (!buffer)
		return (0);
	getcwd(path, PATH_MAX);
	if (!str_append(&buffer, path))
		return (free(buffer), (char *) 0);
	if (!str_append(&buffer, " % "))
		return (free(buffer), (char *) 0);
	return (buffer);
}

int	main(int argc, char **argv, char **env)
{
	t_msh *const	msh = &(t_msh){0};
	char			*prompt;

	(void)argv;
	(void)argc;
	msh->env = env;
	msh->user = get_user();
	while (1)
	{
		prompt = get_prompt(msh);
		if (!prompt)
			mini_panic();
		prompt = readline(prompt);
		free(prompt);
	}
	return (0);
}
