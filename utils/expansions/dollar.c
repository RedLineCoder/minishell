/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:04:50 by moztop            #+#    #+#             */
/*   Updated: 2024/10/04 19:21:24 by moztop           ###   ########.fr       */
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

/* int	set_string(char **str, int size)
{
	char	*line;
	int		index;

	if (!str | !*str)
		return (0);
	line = *str;
	while (size--)
		*line++ = *ps++;
	return (1);
} */

char	*expand_dollar(char *arg)
{
	char	*expanded;
	char	*start;
	int		size;
	int		len;

	size = get_argsize(arg);
	expanded = malloc(sizeof(char) * size + 1);
	while (*arg)
	{
		/* if (*arg == '\'')
		{
			len = pass_quote(&arg, arg + ft_strlen(arg), "'");
		} */
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
