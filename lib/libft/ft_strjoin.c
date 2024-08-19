/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:11:01 by moztop            #+#    #+#             */
/*   Updated: 2023/12/24 13:51:01 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		length;
	int		loop;
	char	*str;

	length = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * length + 1);
	if (!str)
		return (NULL);
	loop = 0;
	while (s1[loop])
	{
		str[loop] = s1[loop];
		loop++;
	}
	loop = 0;
	while (s2[loop])
	{
		str[loop + ft_strlen(s1)] = s2[loop];
		loop++;
	}
	str[loop + ft_strlen(s1)] = '\0';
	return (str);
}
