/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:20:13 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/10 18:30:41 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_environment(t_list **mshenv, char **env)
{
	char	**splitted;

	while (*env)
	{
		splitted = ft_split(*env, '=');
		if (!splitted)
			continue ;
		set_env(mshenv, splitted[0], splitted[1]);
		free_string_array(splitted);
		env++;
	}
}

void	destroy_environment(t_list	*mshenv)
{
	t_list	*lst;
	t_list	*next;

	lst = mshenv;
	while (lst)
	{	
		next = lst->next;
		destroy_env(lst);
		lst = next;
	}
}

char	**get_env_arr(t_list *mshenv)
{
	int			i;
	char		**env;
	t_env		*node;
	const int	size = ft_lstsize(mshenv);

	env = ft_calloc(sizeof(char *), size + 1);
	if (!env)
		return (NULL);
	i = -1;
	while (mshenv)
	{
		node = mshenv->content;
		if (!node->pair)
		{
			mshenv = mshenv->next;
			continue ;
		}
		env[++i] = ft_strdup(node->key);
		if (!env[i] || !str_append(&env[i], "=")
			|| (node->pair && !str_append(&env[i], node->pair)))
			free(env[i--]);
		mshenv = mshenv->next;
	}
	return (env);
}
