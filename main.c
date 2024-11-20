/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/11/20 14:14:50 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_all(t_msh *msh, int exit)
{
	if (msh->line)
	{
		free(msh->line);
		msh->line = NULL;
	}
	if (msh->tree_root)
	{
		clean_tree(msh->tree_root);
		msh->tree_root = NULL;
	}
	if (!exit)
		return ;
	if (msh->env)
	{
		destroy_environment(msh->env);
		msh->env = NULL;
	}
	rl_clear_history();
}

char	*get_prompt(void)
{
	int		size;
	char	path[PATH_MAX];
	char	*prompt;
	char	**path_splitted;

	prompt = ft_strjoin(MSH_TAG, " ");
	getcwd(path, PATH_MAX);
	path_splitted = ft_split(path, '/');
	if (!prompt || !path_splitted)
		return (free(prompt), free_string_array(path_splitted), NULL);
	size = str_arr_size(path_splitted);
	if (size <= 1 && !str_append(&prompt, "/"))
		return (free_string_array(path_splitted), free(prompt), NULL);
	else if (size >= 1 && !str_append(&prompt, path_splitted[size - 1]))
		return (free_string_array(path_splitted), free(prompt), NULL);
	free_string_array(path_splitted);
	if (!str_append(&prompt, " $ "))
		return (free(prompt), NULL);
	return (prompt);
}

int	readline_loop(t_msh *msh)
{
	int		status;
	char	*prompt;

	while ((clean_all(msh, false), true) && !msh->exit_flag)
	{
		handle_signals(WAITING_INPUT);
		prompt = get_prompt();
		if (!prompt)
			return (false);
		msh->line = readline(prompt);
		if ((free(prompt), 1) && !msh->line)
			return (handle_sigint_output(), true);
		handle_signals(NOTHING);
		if (ft_strlen(msh->line) > 0)
		{
			add_history(msh->line);
			status = parser(msh->line, msh->line
					+ ft_strlen(msh->line), &msh->tree_root);
			if (!status && msh->tree_root)
				executor(msh->tree_root, msh);
			else if (msh->tree_root)
				msh->last_status = status;
		}
	}
	return (true);
}

int	main(int argc, char **argv, char **env)
{
	int				flags_set;
	t_msh *const	msh = &(t_msh){0};

	(void)argv, (void)argc;
	flags_set = set_termflags(msh, ADD_ECHOCTL);
	init_environment(&msh->env, env);
	if (!readline_loop(msh))
		msh->last_status = mini_panic(NULL, NULL, EXIT_FAILURE);
	clean_all(msh, true);
	if (flags_set)
		set_termflags(msh, SET_BACK);
	ft_putendl_fd("exit", 1);
	return (msh->last_status);
}
