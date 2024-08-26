/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_old.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 11:21:02 by emyildir          #+#    #+#             */
/*   Updated: 2023/12/18 21:23:44 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	set_initial_fdata(char **data)
{
	*data = (char *) malloc(sizeof(char));
	if (*data)
		**data = '\0';
	return (*data != 0);
}

void	swap_str_and_free(char **str, char *newStr)
{
	free(*str);
	*str = newStr;
}

char	*free_fdata(char	**fdata)
{
	if (*fdata)
	{
		free(*fdata);
		*fdata = 0;
	}
	return (0);
}

char	*get_line(char *file_data)
{
	size_t	len;

	len = 0;
	while (file_data[len])
		if (file_data[len++] == '\n')
			break ;
	return (ft_substr(file_data, 0, len));
}

char	*get_next_line(int fd)
{
	int			bytes_read;
	char		buffer[BUFFER_SIZE + 1];
	char		*line;
	static char	*file_data = 0;

	if (fd < 0 || (!file_data && !set_initial_fdata(&file_data)))
		return (0);
	while (file_data && !ft_strchr(file_data, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == 0 || bytes_read == -1)
			break ;
		buffer[bytes_read] = '\0';
		swap_str_and_free(&file_data, ft_strjoin(file_data, buffer));
	}
	if (!file_data || !*file_data || bytes_read == -1)
		return (free_fdata(&file_data));
	line = get_line(file_data);
	if (!line)
		return (free_fdata(&file_data));
	swap_str_and_free(&file_data, \
	ft_substr(file_data, ft_strlen(line), \
	ft_strlen(file_data) - ft_strlen(line)));
	return (line);
}
