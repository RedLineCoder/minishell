/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:04:50 by moztop            #+#    #+#             */
/*   Updated: 2024/10/15 14:52:26 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_env_var(t_write *wrt, char *arg, t_msh *msh)
{
	int		start;
	char	*envvar;
	char	*key;

	wrt->a_i++;
	start = wrt->a_i;
	while ((ft_isalnum(arg[wrt->a_i]) || arg[wrt->a_i] == '_'))
		wrt->a_i++;
	key = ft_strndup(arg + start, wrt->a_i - start);
	envvar = get_env(msh->env, key);
	if (!envvar)
		return (free(key), "");
	return (free(key), envvar);
}

int	get_size(char *arg, t_msh *msh)
{
	t_write *const	wrt = &(t_write){0};
	char *const		status = ft_itoa(msh->last_status);

	if (!status)
		return (-1);
	while (arg[wrt->a_i])
	{
		if (arg[wrt->a_i] == '"' && !wrt->qs)
			wrt->qd = !wrt->qd;
		if (arg[wrt->a_i] == '\'' && !wrt->qd)
			wrt->qs = !wrt->qs;
		if (arg[wrt->a_i] == '$' && !wrt->qs
			&& (ft_isalnum(arg[wrt->a_i + 1]) || arg[wrt->a_i + 1] == '_'))
			wrt->e_i += ft_strlen(get_env_var(wrt, arg, msh));
		else if (!wrt->qs && arg[wrt->a_i] == '$'
			&& arg[wrt->a_i + 1] == '?' && (wrt->a_i++, wrt->a_i++, 1))
			wrt->e_i += ft_strlen(status);
		else
		{
			wrt->e_i++;
			wrt->a_i++;
		}
	}
	return (free(status), wrt->e_i);
}

int	write_env(char *arg, char *exp, t_list **explst, t_msh *msh)
{
	t_write *const	wrt = &(t_write){0};
	char *const		sts = ft_itoa(msh->last_status);
	char			*env;

	while (arg[wrt->a_i])
	{
		if (arg[wrt->a_i] == '"' && !wrt->qs)
			wrt->qd = !wrt->qd;
		if (arg[wrt->a_i] == '\'' && !wrt->qd)
			wrt->qs = !wrt->qs;
		if (arg[wrt->a_i] == '$' && !wrt->qs
			&& (ft_isalnum(arg[wrt->a_i + 1]) || arg[wrt->a_i + 1] == '_'))
		{
			env = get_env_var(wrt, arg, msh);
			wrt->e_i += ft_strlcpy(exp + wrt->e_i, env, ft_strlen(env) + 1);
			if (!set_exptrack(explst, wrt->e_i - ft_strlen(env), wrt->e_i))
				return (free(sts), ft_lstclear(explst, free), 0);
		}
		else if (arg[wrt->a_i] == '$' && !wrt->qs
			&& arg[wrt->a_i + 1] == '?' && (wrt->a_i++, wrt->a_i++, 1))
			wrt->e_i += ft_strlcpy(exp + wrt->e_i, sts, ft_strlen(sts) + 1);
		else
			exp[wrt->e_i++] = arg[wrt->a_i++];
	}
	return (free(sts), 1);
}

char	*expand_dollar(char *arg, t_list **explst, t_msh *msh)
{
	char		*exp;
	int const	size = get_size(arg, msh);

	if (size == -1)
		return (NULL);
	exp = ft_calloc(sizeof(char), size + 1);
	if (!exp)
		return (NULL);
	if (!write_env(arg, exp, explst, msh))
		return (*explst = NULL, NULL);
	return (exp);
}
