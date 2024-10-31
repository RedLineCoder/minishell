/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 23:01:32 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/30 15:07:19 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_part	ft_divide(char *s, char *e, t_tokens tkn, int rev)
{
	t_part	ln;
	char	*ts;
	char	*te;

	ln.lfts = s;
	while (s && s != e)
	{
		pass_block(s, &s, e);
		if (get_token(&s, &e, &ts, &te) == tkn)
		{
			ln.lfte = ts;
			ln.rghts = te;
			if (!rev)
				break ;
		}
	}
	ln.rghte = e;
	return (ln);
}

int	str_arr_size(char **arr)
{
	int	size;

	size = -1;
	while (arr[++size])
		;
	return (size);
}

void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

int	str_append(char **s1, char const *s2)
{
	int		s1_len;
	int		s2_len;
	char	*str;

	s1_len = ft_strlen(*s1);
	s2_len = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!str)
		return (0);
	ft_strlcpy(str, *s1, s1_len + 1);
	ft_strlcpy(str + s1_len, s2, s2_len + 1);
	*s1 = (free(*s1), str);
	return (1);
}

char	*str_include(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if ((unsigned char) s[i] == c)
			return ((char *) s + i);
		i++;
	}
	return (0);
}
