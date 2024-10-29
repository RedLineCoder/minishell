/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/10/29 19:22:08 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_panic(char *title, char *content, int status)
{
	ft_putstr_fd(ERR_TAG, STDERR_FILENO);
	if (title)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(title, STDERR_FILENO);
	}
	ft_putstr_fd(": ", STDERR_FILENO);
	if (content)
		ft_putstr_fd(content, STDERR_FILENO);
	else
		perror("");
	return (status);
}

void	clean_all(t_msh *msh, int exit)
{
	free(msh->line);
	clean_tree(msh->tree_root);
	if (!exit)
		return ;
	rl_clear_history();
	destroy_environment(msh->env);
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
	if (size <= 1 && (!str_append(&prompt, "/")))
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

	while (!msh->exit_flag)
	{
		handle_signals(WAITING_INPUT);
		prompt = get_prompt();
		if (!prompt)
			return (false);
		if (isatty(fileno(stdin)))
			msh->line = readline(prompt);
		else
		{
			char	*line ;
			line = get_next_line(fileno(stdin));
			msh->line = ft_strtrim(line, "\n");
			free(line);
		}
		if (!msh->line)
			return (ft_putchar_fd('\n', STDOUT_FILENO), true);
		if ((free(prompt), 1) && !msh->line)
			return (ft_putchar_fd('\n', STDOUT_FILENO), true);
		handle_signals(NOTHING);
		if (msh->line && ft_strlen(msh->line) > 0)
		{
			add_history(msh->line);
			status = parser(msh->line, msh->line
					+ ft_strlen(msh->line), &msh->tree_root);
			if (!status)
				executor(msh->tree_root, msh);
			else if ((clean_all(msh, 0), 1))
				msh->last_status = status;
		}
	}
	return (true);
}

int	main(int argc, char **argv, char **env)
{
	t_msh *const	msh = &(t_msh){0};

	(void)argv, (void)argc;
	set_termflags();
	handle_signals(NOTHING);
	init_environment(&msh->env, env);
	if (!readline_loop(msh))
		msh->last_status = mini_panic(ERR_TAG, NULL, EXIT_FAILURE);
	rl_clear_history();
	destroy_environment(msh->env);
	ft_putendl_fd("exit", 1);
	return (msh->last_status);
}
