/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_by_emur.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 16:41:56 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/21 03:54:55 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	match_wildcard(char *str, char *exp, int first)
{
	int const	hidden_flag = first && *exp == '.';

	if (!exp || !str || ((first && *str == '.') && !hidden_flag))
		return (false);
	while (*exp && *exp != WILDCARD)
	{
		if (*str != *exp && !(*exp == -1 && *str == WILDCARD))
			return (false);
		str++;
		exp++;
	}
	if (*exp == WILDCARD)
	{
		while (*exp == WILDCARD)
			exp++;
		if (!*exp)
			return (true);
		if (!ft_strchr(str, *exp))
			return (false);
		while (*str)
			if (match_wildcard(str++, exp, false))
				return (true);
		return (false);
	}
	return (!*exp && !*str);
}

int	handle_wildcards(t_list **arglst, char *arg)
{
	int				count;
	char			*dirname;
	struct dirent	*dp;
	DIR *const		current_dir = opendir(".");

	count = 0;
	dp = readdir(current_dir);
	while (dp)
	{
		if (match_wildcard(dp->d_name, arg, true))
		{
			dirname = ft_strdup(dp->d_name);
			if (!dirname)
				return (-1);
			if (!lst_addback_content(arglst, dirname))
				return (-1);
			count++;
		}
		dp = readdir(current_dir);
	}
	closedir(current_dir);
	return (count);
}

int	format_dollar(t_msh *msh, char **arg, char *pair, int start)
{
	char		*newarg;
	char		*str;
	char *const	env = get_env(msh->env, pair);

	if (!(*arg)[start + 1])
		str = ft_strdup("$");
	else if (env)
		str = ft_strdup(env);
	else if (!ft_strncmp(pair, "?", 2))
		str = ft_itoa(msh->last_status);
	else
		str = ft_strdup("");
	if (!str)
		return (false);
	newarg = ft_calloc(sizeof(char), ft_strlen(*arg) \
	+ ft_strlen(str) - ft_strlen(pair));
	if (!newarg)
		return (free(str), false);
	ft_memcpy(newarg, *arg, start);
	ft_memcpy(newarg + start, str, ft_strlen(str));
	ft_memcpy(newarg + start + ft_strlen(str), *arg + start + \
	ft_strlen(pair) + 1, ft_strlen(*arg) - start - ft_strlen(pair) - 1);
	free(*arg);
	*arg = newarg;
	return (free(str), 1);
}

int	handle_dollars(t_msh *msh, char **arg)
{
	int		i;
	int		j;
	char	*pair;

	i = -1;
	while ((*arg)[++i])
	{
		if ((*arg)[i] == DOLLAR)
		{
			j = i;
			while ((*arg)[j + 1] && (*arg)[j + 1] != ' '
				&& (*arg)[j + 1] != QUOTE && !ft_strchr(QUOTES, (*arg)[j + 1])
				&& (*arg)[j + 1] != DOLLAR)
				j++;
			pair = ft_strndup(*arg + i + 1, j - i);
			if (!pair || !format_dollar(msh, arg, pair, i))
				return (false);
			i = -1;
		}
	}
	return (true);
}

void	set_expander_metas(char *arg)
{
	char	quote;

	while (*arg)
	{
		if (ft_strchr(QUOTES, *arg))
		{
			if (!quote)
			{
				quote = *arg;
				*arg = QUOTE;
			}
			else if (quote == *arg)
			{
				quote = 0;
				*arg = QUOTE;
			}
		}
		else if (*arg == '*' && !quote)
			*arg = WILDCARD;
		else if (*arg == '$' && quote != '\'')
			*arg = DOLLAR;
		arg++;
	}
}

void	remove_quotes(char *arg)
{
	int		i;
	int		diff;
	char	chr;
	int		quoted;

	quoted = false;
	diff = 0;
	i = -1;
	while (arg[++i])
	{
		chr = arg[i];
		if (chr == QUOTE && ++diff)
			quoted = !quoted;
		if (chr != QUOTE)
		{
			if (chr == '*' && !quoted)
				chr = WILDCARD;
			arg[i - diff] = chr;
		}
	}
	arg[i - diff] = '\0';
}

t_list	*expander(t_list *args, t_msh *msh)
{
	char	*temp;
	char	*arg;
	t_list	*newargs;

	newargs = NULL;
	while (args)
	{
		arg = ft_strdup(args->content);
		if (!arg)
			return (ft_lstclear(&newargs, free), NULL);
		set_expander_metas(arg);
		if (!handle_dollars(msh, &arg))
			return (free(arg), ft_lstclear(&newargs, free), NULL);
		remove_quotes(arg);
		if (!handle_wildcards(&newargs, arg))
		{
			temp = arg - 1;
			while (*(++temp))
				if (*temp == WILDCARD)
					*temp = '*';
			lst_addback_content(&newargs, arg);
		}
		args = args->next;
	}
	return (newargs);
}
