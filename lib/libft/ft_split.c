/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 17:09:57 by moztop            #+#    #+#             */
/*   Updated: 2023/12/25 14:59:39 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_countwords(char const *s, char c)
{
	int		loop;
	int		count;
	int		newword;

	loop = 0;
	count = 0;
	newword = 1;
	while (s[loop])
	{
		if (s[loop] != c && newword == 1)
		{
			count++;
			newword = 0;
		}
		if (s[loop] == c && newword == 0)
			newword = 1;
		loop++;
	}
	return (count);
}

static	void	ft_clean(char **table)
{
	int	loop;

	loop = 0;
	while (table[loop])
		free(table[loop++]);
	free(table);
}

static	char	**word_split(char **table, char const *s, char c)
{
	int		loop;
	int		index;
	int		ptr;

	loop = 0;
	index = 0;
	while (s[loop])
	{
		while (s[loop] == c)
			loop++;
		ptr = loop;
		while (s[loop] != c && s[loop])
			loop++;
		if (loop > ptr)
		{
			table[index] = ft_substr(s, ptr, loop - ptr);
			if (!table[index])
				return (ft_clean(table), NULL);
			index++;
		}
	}
	table[index] = NULL;
	return (table);
}

char	**ft_split(char const *s, char c)
{
	char	**table;

	if (!s)
		return (NULL);
	table = (char **)malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (!table)
		return (NULL);
	return (word_split(table, s, c));
}
