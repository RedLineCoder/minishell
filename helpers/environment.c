/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:20:13 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/30 21:09:59 by emyildir         ###   ########.fr       */
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

int	unset_env(t_list **root, char *key)
{
	t_list	*lst;
	t_list	*target;

	lst = *root;
	target = get_env_node(*root, key);
	if (!target)
		return (true);
	if (lst == target)
	{
		(*root) = lst->next;
		destroy_env(lst);
	}
	else
	{
		while (lst->next != target)
			lst = lst->next;
		lst->next = target->next;
		destroy_env(target);
	}
	return (true);
}

int	set_env(t_list **root, char *key, char *pair)
{
	t_list	*lst;
	t_env	*env;

	lst = get_env_node(*root, key);
	if (lst)
		env = lst->content;
	else
	{
		env = ft_calloc(sizeof(t_env), 1);
		lst = ft_lstnew(env);
		if (!env || !lst)
			return (ft_lstdelone(lst, free), \
			mini_panic("env", NULL, false));
		ft_lstadd_back(root, lst);
		env->key = ft_strdup(key);
		if (!env->key)
			return (ft_lstdelone(lst, free), \
			mini_panic("env", NULL, false));
	}
	free(env->pair);
	env->pair = NULL;
	if (pair)
		env->pair = ft_strdup(pair);
	return (true);
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
