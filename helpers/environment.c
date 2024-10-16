/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:20:13 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/16 17:57:22 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_environment(t_list **mshenv, char **env)
{
	char	*key;
	char	*pair;
	char	*shlvl;
	
	while (*env)
	{
		key = *env;
		pair = ft_strchr(key, '=');
		if (pair)
			*pair++ = '\0';
		set_env(mshenv, key, pair);
		env++;
	}
	shlvl = get_env(*mshenv, "SHLVL");
	if (shlvl)
	{
		shlvl = ft_itoa(ft_atoi(shlvl) + 1);
		if (shlvl)
		{
			set_env(mshenv, "SHLVL", shlvl);
			free(shlvl);
		}
	}
	else
		set_env(mshenv, "SHLVL", "1");
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
