/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 18:29:40 by moztop            #+#    #+#             */
/*   Updated: 2024/09/10 21:43:02 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*char1;
	unsigned char	*char2;

	char1 = (unsigned char *)s1;
	char2 = (unsigned char *)s2;
	while (*char1 == *char2 && *char1 && n)
	{
		char1++;
		char2++;
		n--;
	}
	if (n)
		return (*char1 - *char2);
	else
		return (0);
}
