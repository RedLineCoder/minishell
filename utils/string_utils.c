/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 23:01:32 by emyildir          #+#    #+#             */
/*   Updated: 2024/09/20 19:26:39 by moztop           ###   ########.fr       */
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

char	*pass_block(char *bs, char *pe)
{
	int		sem_block;

	sem_block = 1;
	while (bs != pe && *bs && sem_block)
	{
		bs++;
		if (*bs == ')')
			sem_block--;
		if (*bs == '(')
			sem_block++;
	}
	return (bs);
}

t_lnsplit	ft_lnsplit(char *line, char *end)
{
	t_lnsplit 	ln;
	int			sem;

	sem = 0;
	ln.lfts = line;
	while (line && (end && line != end))
	{
		if (*line == '(' || ft_strchr(QUOTES ,*line))
			line = pass_block(line, end);
		if (!ft_strncmp(line, "&&", 2) || !ft_strncmp(line, "||", 2))
		{
			ln.lfte = line;
			line += 2;
			ln.rghts = line;
		}
		if (*line == '(' || ft_strchr(QUOTES ,*line))
			line = pass_block(line, end);
		line++;
	}
	ln.rghte = line;
	return (ln);
}

t_lnsplit	ft_lnsplit2(char *line, char *end)
{
	t_lnsplit 	ln;
	int			sem;

	sem = 0;
	ln.lfts = line;
	while (line && (end && line != end))
	{
		if (*line == '(')
			line = pass_block(line, end);
		if (*line == '|')
		{
			ln.lfte = line;
			line += 2;
			ln.rghts = line;
		}
		if (*line == '(')
			line = pass_block(line, end);
		line++;
	}
	ln.rghte = line;
	return (ln);
}
