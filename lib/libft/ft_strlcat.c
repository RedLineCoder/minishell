/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 13:55:21 by moztop            #+#    #+#             */
/*   Updated: 2023/12/24 13:44:08 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	unsigned int	l;
	unsigned int	dest_size;

	dest_size = ft_strlen(dst);
	if (size <= dest_size)
		return (size + ft_strlen(src));
	l = 0;
	while (src[l] != '\0' && dest_size + l + 1 < size)
	{
		dst[dest_size + l] = src[l];
		l++;
	}
	dst[dest_size + l] = '\0';
	return (dest_size + ft_strlen(src));
}
