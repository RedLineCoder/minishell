/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moztop <moztop@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 18:13:04 by moztop            #+#    #+#             */
/*   Updated: 2023/12/25 16:31:20 by moztop           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static	int	ft_length(long n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		len++;
		n *= -1;
	}
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*handle(long num, char *table, int len)
{
	int		index;
	int		size;

	index = 0;
	if (num < 0)
	{
		table[0] = '-';
		num *= -1;
		index = 1;
	}
	size = len - 1;
	while (size >= index)
	{
		table[size--] = (num % 10) + '0';
		num /= 10;
	}
	table[len] = '\0';
	return (table);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*table;
	long	num;

	num = n;
	len = ft_length(num);
	table = (char *)malloc(sizeof(char) * len + 1);
	if (!table)
		return (NULL);
	return (handle(num, table, len));
}
