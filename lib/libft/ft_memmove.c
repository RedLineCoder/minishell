/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 18:22:32 by moztop            #+#    #+#             */
/*   Updated: 2023/12/25 16:56:52 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*destination;
	unsigned const char	*source;

	destination = (unsigned char *)dest;
	source = (unsigned char *)src;
	if (dest < src)
		return (ft_memcpy(dest, src, n));
	while (n-- && (src || dest))
		destination[n] = source[n];
	return (dest);
}
