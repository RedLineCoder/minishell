/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 16:41:37 by moztop            #+#    #+#             */
/*   Updated: 2023/12/24 17:12:35 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_memcmp(void *dest, const void *src, size_t n)
{
	unsigned char	*destination;
	unsigned char	*source;
	size_t			loop;

	destination = (unsigned char *)dest;
	source = (unsigned char *)src;
	loop = 0;
	while (loop < n && destination[loop] == source[loop])
	{
		loop++;
	}
	if (loop < n)
		return (destination[loop] - source[loop]);
	else
		return (0);
}
