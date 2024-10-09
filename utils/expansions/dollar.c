/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:04:50 by moztop            #+#    #+#             */
/*   Updated: 2024/10/09 18:19:30 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_envvar(char **dollar)
{
	char	*start;
	char	*end;
	char	*envvar;
	char	*var;

	(*dollar)++;
	start = *dollar;
	while ((ft_isalnum(**dollar) || **dollar == '_') && *dollar)
		(*dollar)++;
	end = *dollar;
	var = ft_strndup(start, end - start);
	envvar = getenv(var);
	if (!envvar)
		return ("");
	return (free(var), envvar);
}

int	get_size(char *arg)
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
			len += ft_strlen(get_envvar(&arg));
			continue ;
		}
		arg++;
		len++;
	}
	return (len);
}

int	print_env(char *arg, char *exp, t_list **explst)
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
			envvar = get_envvar(&arg);
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

char	*expand_dollar(char *arg, t_list **explst)
{
	char	*exp;

	exp = ft_calloc(sizeof(char), get_size(arg) + 1);
	if (!exp)
		return (NULL);
	if (!print_env(arg, exp, explst))
	{
		ft_lstclear(explst, free);
		*explst = NULL;
		return (NULL);
	}
	return (exp);
}
