/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 18:29:40 by moztop            #+#    #+#             */
/*   Updated: 2023/12/24 14:05:29 by moztop           ###   ########.fr       */
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
