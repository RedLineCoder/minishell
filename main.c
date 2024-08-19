/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/08/19 22:11:51 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	mini_panic()
{
	exit(1);
}

char	*get_prompt(t_msh *msh)
{
	char	path[PATH_MAX];
	char	*usr;

	getcwd(path, PATH_MAX);
	if (ft_strncmp("/Users", path, 6) == 0)
		usr = usr + 6 + ft_strlen(msh->user) + 1;
	return (ft_strjoin("~", usr));
}

int	main(int argc, char **argv, char **env)
{
	t_msh *const	msh = &(t_msh){0};
	char			*prompt;

	(void)argv;
	(void)argc;
	msh->env = env;
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


