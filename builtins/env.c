/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:15:28 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/05 14:13:21 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(t_list *lst, int quotes, int hide_null)
{
	t_env	*env;
	
	while (lst)
	{
		env = lst->content;
		if (env->key && (!hide_null || env->pair))
		{
			printf("%s", env->key);
			if (env->pair)
			{
				if (quotes)
					printf("=\"");
				printf("%s", env->pair);
				if (quotes)
					printf("\"");
			}
			printf("\n");
		}
		lst = lst->next;
	}
}

int	builtin_env(int args_size, char **args, t_msh *msh)
{	
	(void)args;
	(void)args_size;
	print_env(msh->env, false, true);
	return(EXIT_SUCCESS);
}