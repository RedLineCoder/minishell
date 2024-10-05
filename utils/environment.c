/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 07:19:33 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/05 13:49:37 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	destroy_env(t_list *lst)
{
	t_env	*env;

	env = lst->content;
	if (env)
	{
		free(env->key);
		free(env->pair);
	}
	ft_lstdelone(lst, free);
}

t_list	*get_env_node(t_list *lst, char *key)
{
	t_env	*env;
	
	while (lst)
	{
		env = lst->content;
		if (!ft_strncmp(key, env->key, ft_strlen(key) + 1))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

char	*get_env(t_list *root, char *key)
{
	t_list	*node;
	t_env	*env;
	
	node = get_env_node(root, key);
	if (!node)
		return (NULL);
	env = node->content;
	return (env->pair);
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
			mini_panic("env", NULL, false, -1), false);
		ft_lstadd_back(root, lst);
		env->key = ft_strdup(key);
		if (!env->key)
			return (ft_lstdelone(lst, free), \
			mini_panic("env", NULL, false, -1), false);
	}
	free(env->pair);
	env->pair = NULL;
	if (pair)
		env->pair = ft_strdup(pair);
	return (true);
}

