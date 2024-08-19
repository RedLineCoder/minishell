/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:02:10 by moztop            #+#    #+#             */
/*   Updated: 2023/12/24 17:33:44 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	loop;
	char	*substr;

	if (!s)
		return (NULL);
	if ((unsigned int)ft_strlen(s) < start)
		return (ft_strdup(""));
	if (ft_strlen(s) < len + start)
		len = ft_strlen(s) - start;
	substr = (char *)malloc((sizeof(char) * len + 1));
	if (!substr)
		return (NULL);
	loop = 0;
	while (loop < len)
	{
		substr[loop] = s[start + loop];
		loop++;
	}
	substr[loop] = '\0';
	return (substr);
}
