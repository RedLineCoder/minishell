/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:20:28 by emyildir          #+#    #+#             */
/*   Updated: 2024/10/13 19:03:44 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Was working but needed to rewrite this due errors causing by PATH variable.
char	**read_fd(int fd)
{
	char	*data;
	char	*line;

	data = ft_strdup("");
	if (!data)
		return (NULL);
	line = get_next_line(fd);
	while (line)
	{
		str_append(&data, line);
		free(line);
		line = get_next_line(fd);
	}
	return (ft_split(data, '\n'));
}

char	*parse_line(char *path, char seperator, int index, char *val)
{
	int	i;
	int fd;
	char	*line;
	char	**splitted_line;
	char	**data;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	line = NULL;
	i = -1;
	data = read_fd(fd);
	while (!line && data && data[++i])
	{
		splitted_line = ft_split(data[i], seperator);
		if (splitted_line && index < str_arr_size(splitted_line) 
				&& !ft_strncmp(splitted_line[index], val, ft_strlen(val) + 1))
			line = ft_strdup(data[i]);
		free_string_array(splitted_line);
	}
	free_string_array(data);
	close(fd);
	return (line);
}

char	*get_username_by_uid(char *uid)
{
	char		*username;
  char    **splitted;
	char		*const line = parse_line("/etc/passwd", ':', 2, uid);

  if (!line)
      return (NULL);
  splitted = ft_split(line, ':');
  if (!splitted)
      return (free(line), NULL);
  username = ft_strdup(splitted[0]);
  free_string_array(splitted);
	return (free(line), username);
}	

char	*get_user()
{
  int   i;
  char  *sep;
  char  *uid;
  char  *username;
	char  *line = parse_line("/proc/self/status", ':', 0, "Uid");
	
  sep = ft_strchr(line, ':');
  if (!sep)
      return (NULL);
  uid = sep + 1;
  i = 0;
  while (uid[i] == 9)
      i++;
  uid += i;
  while (uid[i] && uid[i] != 9)
      i++;
  uid[i] = '\0';
  username = get_username_by_uid(uid);
  free(line);
	return (username);
}
