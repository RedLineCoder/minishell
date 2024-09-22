/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 23:01:32 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/22 16:32:31 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strndup(char *src, int size)
{
	char	*const str = malloc(sizeof(char) * (size + 1));
	
	if (!str)
		return (NULL);
	str[size] = '\0';
	while (--size >= 0)
		str[size] = src[size];
	return (str);
}

int	str_arr_size(char **arr)
{
	int		size;

	size = -1;
	while (arr[++size])
		;
	return (size);
}

void	free_string_array(char **arr)
{
	int		i;

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
	str = (char *) malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!str)
		return (0);
	ft_strlcpy(str, *s1, s1_len + 1);
	ft_strlcpy(str + s1_len, s2, s2_len + 1);
	*s1 = (free(*s1), str);
	return (1);
}

t_lnsplit	ft_lnsplit(char *line, char *end, t_tokens token, int reverse)
{
	t_lnsplit 	ln;
	char		*ts;
	char		*te;

	ln.lfts = line;
	while (line && line != end)
	{
		line = pass_block(line, end);
		if (get_token(&line, &end, &ts, &te) == token)
		{
			ln.lfte = ts;
			ln.rghts = te;
			if (!reverse)
				break ;
		}
	}
	ln.rghte = end;
	return (ln);
}
