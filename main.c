/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/08/26 08:06:40 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_panic(void)
{
	exit(1);
}

char	*get_prompt(t_msh *msh)
{
	int		size;
	char	path[PATH_MAX];
	char	*prompt;
	char	**path_splitted;

	getcwd(path, PATH_MAX);
	prompt = ft_strjoin(msh->user, " ");
	path_splitted = ft_split(path, '/');
	if (!prompt || !path_splitted)
		return (free(prompt), free_string_array(path_splitted), NULL);
	size = str_arr_size(path_splitted);
	if (size <= 1 && (!str_append(&prompt, "/")))
		return (free_string_array(path_splitted), free(prompt), NULL);
	else if (size == 2 && (!ft_strncmp(path_splitted[0], "Users", 6)
			&& !ft_strncmp(path_splitted[1], msh->user, ft_strlen(msh->user))))
	{
		if (!str_append(&prompt, "~"))
			return (free_string_array(path_splitted), free(prompt), NULL);
	}
	else if (size >= 1 && !str_append(&prompt, path_splitted[size - 1]))
		return (free_string_array(path_splitted), free(prompt), NULL);
	free_string_array(path_splitted);
	if (!str_append(&prompt, " % "))
		return (free(prompt), NULL);
	return (prompt);
}

int	main(int argc, char **argv, char **env)
{
	t_msh *const	msh = &(t_msh){0};
	char			*prompt;
	char			*line;

	(void)argv;
	(void)argc;
	msh->env = env;
	msh->user = get_user();
	while (1)
	{
		prompt = get_prompt(msh);
		if (!prompt)
			mini_panic();
		line = readline(prompt);
		parser(line);
		free(line);
		free(prompt);
	}
	return (0);
}
