/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 07:19:33 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/31 15:57:41 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_bonus.h"

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
