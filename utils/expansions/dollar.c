/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:04:50 by moztop            #+#    #+#             */
/*   Updated: 2024/10/04 15:15:31 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_envvar(char *start, char *end)
{
	char	*envvar;
	char	*var;

	var = ft_strndup(start, end - start);
	envvar = getenv(var);
	return (free(var), envvar);
}

char	*get_argsize(char *arg)
{
	int		len;
	char	*start;
	char	*end;

	while (*arg)
	{
		len += pass_quote(&arg, arg + ft_strlen(arg), "\'");
		if (*arg++ == '$')
		{
			start = arg;
			while (ft_isdigit(arg) || ft_isalpha(arg) || *arg == '_')
				arg++;
			end = arg;
			len += ft_strlen(get_envvar(start, end));
		}
	}
	return (len);
}

char	*expand_dollar(char *arg)
{
	char	*expanded;
	int		size;
	int		index;

	size = get_argsize(arg);
	expanded = ft_calloc(sizeof(char), size + 1);
	index = -1;
	while (++index < size)
	{
		if (*arg == '\'')
		{
			expanded[index++] = *arg++;
			while (index < size && *arg && *arg != '\'')
				expanded[index++] = *arg++;
		}
	}
}

/* char	*get_var(char *arg)
{
	char	*envvar;
	char	*var;
	char	*start;
	char	*end;

	while (*arg)
	{
		if (*arg == '\'')
			arg = pass_quote(arg, arg + ft_strlen(arg));
		if (*arg == '$')
		{
			arg++;
			start = arg;
			while (ft_isdigit(arg) || ft_isalpha(arg) || *arg == '_')
				arg++;
			end = arg;
			break ;
		}
		arg++;
	}
	var = ft_strndup(start, end - start);
	envvar = getenv(var);
	return (free(var), envvar);
} */
