/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:04:50 by moztop            #+#    #+#             */
/*   Updated: 2024/10/26 19:05:55 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_env_var(t_write *wrt, t_msh *msh, char *arg, char *exp)
{
	char *const		status = ft_itoa(msh->last_status);
	int				start;
	int				expsize;
	char			*envvar;
	char			*key;

	if (arg[++wrt->a_i] == '?')
	{
		expsize = ft_strlen(status);
		if (exp)
			ft_strlcpy(&exp[wrt->e_i], status, expsize + 1);
		return (free(status), wrt->a_i++, expsize);
	}
	start = wrt->a_i;
	while ((ft_isalnum(arg[wrt->a_i]) || arg[wrt->a_i] == '_'))
		wrt->a_i++;
	key = ft_strndup(&arg[start], wrt->a_i - start);
	envvar = get_env(msh->env, key);
	expsize = ft_strlen(envvar);
	if (exp)
		ft_strlcpy(&exp[wrt->e_i], envvar, expsize + 1);
	return (free(key), free(status), expsize);
}

int	get_size(char *arg, t_msh *msh)
{
	t_write *const	wrt = &(t_write){0};

	while (arg[wrt->a_i])
	{
		track_quotes(wrt, NULL, arg);
		if (arg[wrt->a_i] == '$' && wrt->qc != '\''
			&& (ft_isalnum(arg[wrt->a_i + 1])
				|| arg[wrt->a_i + 1] == '_' || arg[wrt->a_i + 1] == '?'))
			wrt->e_i += get_env_var(wrt, msh, arg, NULL);
		else
		{
			wrt->e_i++;
			wrt->a_i++;
		}
	}
	return (wrt->e_i);
}

char	*expand_dollar(char *arg, t_list **explst, t_msh *msh)
{
	t_write *const	wrt = &(t_write){0};
	char			*exp;
	int				start;

	exp = ft_calloc(sizeof(char), get_size(arg, msh) + 1);
	if (!exp)
		return (NULL);
	while (arg[wrt->a_i])
	{
		track_quotes(wrt, NULL, arg);
		if (arg[wrt->a_i] == '$' && wrt->qc != '\''
			&& (ft_isalnum(arg[wrt->a_i + 1])
				|| arg[wrt->a_i + 1] == '_' || arg[wrt->a_i + 1] == '?'))
		{
			start = wrt->e_i;
			wrt->e_i += get_env_var(wrt, msh, arg, exp);
			if (!set_exptrack(explst, start, wrt->e_i))
				return (ft_lstclear(explst, free), NULL);
		}
		else
			exp[wrt->e_i++] = arg[wrt->a_i++];
	}
	return (exp);
}
