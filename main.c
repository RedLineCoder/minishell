/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:05 by moztop            #+#    #+#             */
/*   Updated: 2024/10/27 16:36:46 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_job	job = NOTHING;

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
	//msh->user;
}

int	readline_loop(t_msh *msh)
{
	int		status;
	char	*prompt;

	while (!msh->exit_flag)
	{
		job = WAITING_INPUT;
		prompt = get_prompt();
		if (!prompt)
			return (false);
		msh->line = readline(prompt);
		free(prompt);
		if (!msh->line)
			return (false);
		if (msh->line && ft_strlen(msh->line) > 0)
		{
			add_history(msh->line);
			status = parser(msh->line, msh->line + ft_strlen(msh->line), &msh->tree_root);
			if (!status)
				executor(msh->tree_root, msh);
			else
				msh->last_status = status;
			clean_all(msh, 0);
		}
		job = NOTHING;
	}
	return (true);
}

int	main(int argc, char **argv, char **env)
{
	t_msh *const	msh = &(t_msh){0};

	(void)argv, (void)argc;
	init_environment(&msh->env, env);
	handle_signals();
	if (!readline_loop(msh))
		msh->last_status = mini_panic(ERR_TAG, NULL, EXIT_FAILURE);
	rl_clear_history();
	destroy_environment(msh->env);
	//printf("exit\n");
	return (msh->last_status);
}
