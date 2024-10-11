/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:04:50 by moztop            #+#    #+#             */
/*   Updated: 2024/10/11 15:01:29 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_envvar(char **dollar, char *status)
{
	char	*start;
	char	*end;
	char	*envvar;
	char	*var;

	(*dollar)++;
	if (**dollar == '$')
		return ("");
	else if (**dollar == '?')
	{
		(*dollar)++;
		return (status);
	}
	start = *dollar;
	while ((ft_isalnum(**dollar) || **dollar == '_') && *dollar)
		(*dollar)++;
	end = *dollar;
	var = ft_strndup(start, end - start);
	envvar = getenv(var);
	if (!envvar)
		return (free(var), "");
	return (free(var), envvar);
}

int	get_size(char *arg, char *status)
{
	int	len;
	int	dq;

	len = 0;
	dq = 0;
	while (*arg)
	{
		if (*arg == '"')
			dq = !dq;
		if (!dq)
			len += pass_quote(&arg, arg + ft_strlen(arg), "\'");
		if (*arg == '$')
		{
			len += ft_strlen(get_envvar(&arg, status));
			continue ;
		}
		arg++;
		len++;
	}
	return (len);
}

int	write_env(char *arg, char *exp, char *status, t_list **explst)
{
	char	*envvar;
	char	qs;
	char	qd;
	char	*start;

	qs = 0;
	qd = 0;
	while (*arg)
	{
		if (*arg == '"')
			qd = !qd;
		if (*arg == '\'' && !qd)
			qs = !qs;
		if (*arg == '$' && !qs)
		{
			envvar = get_envvar(&arg, status);
			start = exp;
			exp += ft_strlcpy(exp, envvar, ft_strlen(envvar) + 1);
			if (!set_exptrack(explst, start, exp - 1))
				return (0);
		}
		else
			*exp++ = *arg++;
	}
	return (1);
}

char	*expand_dollar(char *arg, char *status, t_list **explst)
{
	char	*exp;

	if (!status)
		return (NULL);
	exp = ft_calloc(sizeof(char), get_size(arg, status) + 1);
	if (!exp)
		return (free(status), NULL);
	if (!write_env(arg, exp, status, explst))
	{
		ft_lstclear(explst, free);
		*explst = NULL;
		return (free(status), NULL);
	}
	return (free(status), exp);
}
