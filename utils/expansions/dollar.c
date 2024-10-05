/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:04:50 by moztop            #+#    #+#             */
/*   Updated: 2024/10/04 21:21:44 by moztop           ###   ########.fr       */
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
	while (ft_isdigit(**dollar) 
		|| ft_isalpha(**dollar) || **dollar == '_')
		(*dollar)++;
	end = *dollar;
	var = ft_strndup(start, end - start);
	envvar = getenv(var);
	if (!envvar)
		return (" ");
	return (free(var), envvar);
}

int	get_size(char *arg)
{
	int		len;

	len = 0;
	while (*arg)
	{
		len += pass_quote(&arg, arg + ft_strlen(arg), "\'");
		if (*arg == '$')
			len += ft_strlen(get_envvar(&arg));
		arg++;
		len++;
	}
	return (len);
}

char	*expand_dollar(char *arg)
{
	char	*expanded;
	char	*envvar;
	int		expand;
	int		index;

	expand = 1;
	index = 0;
	expanded = ft_calloc(sizeof(char), get_size(arg) + 1);
	if (!expanded)
		return (NULL);
	while (*arg)
	{
		if (*arg == '\'')
			expand = !expand;
		if (*arg == '$' && expand)
		{
			envvar = get_envvar(&arg);
			ft_strlcpy(expanded + index, envvar, ft_strlen(envvar));
			index += ft_strlen(envvar) - 1;
			continue ;
		}
		expanded[index] = *arg++;
		index++;
	}
	return (expanded);
}
