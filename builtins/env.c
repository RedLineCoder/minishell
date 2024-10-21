/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:15:28 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/21 22:15:26 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(t_list *lst, int quotes_flags, int null_flag)
{
	t_env	*env;

	while (lst)
	{
		env = lst->content;
		if (env->key && (!null_flag || env->pair))
		{
			ft_putstr_fd(env->key, STDOUT_FILENO);
			if (env->pair)
			{
				ft_putstr_fd("=", STDOUT_FILENO);
				if (quotes_flags)
					ft_putstr_fd("\"", STDOUT_FILENO);
				ft_putstr_fd(env->pair, STDOUT_FILENO);
				if (quotes_flags)
					ft_putstr_fd("\"", STDOUT_FILENO);
			}
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		lst = lst->next;
	}
}

int	builtin_env(int args_size, char **args, t_msh *msh)
{
	(void)args;
	(void)args_size;
	print_env(msh->env, false, true);
	return (EXIT_SUCCESS);
}
