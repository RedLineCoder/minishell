/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 18:29:48 by moztop            #+#    #+#             */
/*   Updated: 2023/12/25 15:11:13 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	chr;
	unsigned char	*ptr;

	chr = (unsigned char)c;
	ptr = NULL;
	while (*s)
	{
		if (*s == chr)
			ptr = (unsigned char *)s;
		s++;
	}
	if (*s == chr)
		return ((char *)s);
	return ((char *)ptr);
}
