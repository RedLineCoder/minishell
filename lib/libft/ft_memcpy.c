/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 16:57:57 by moztop            #+#    #+#             */
/*   Updated: 2023/12/12 20:38:46 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*destination;
	unsigned const char	*source;

	destination = (unsigned char *)dest;
	source = (unsigned char *)src;
	while (n-- && (src || dest))
		*destination++ = *source++;
	return (dest);
}
