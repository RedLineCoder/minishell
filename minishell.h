/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emyildir <emyildir@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 20:17:12 by moztop            #+#    #+#             */
/*   Updated: 2024/08/26 08:03:34 by emyildir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
	
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <unistd.h>
# include <stdbool.h>
# include "lib/libft/libft.h"
# include "lib/gnl/get_next_line.h"


typedef	struct s_msh
{
	char	*user;
	char	**env;
}	t_msh;

typedef struct	s_cmd
{
	int	type;
}	t_cmd;

char	*get_user();
char	*get_path(char *command);
int		str_append(char **s1, char const *s2);
int		str_arr_size(char **arr);
void	parser(char *prompt);
void	free_string_array(char **arr);
void	execute_command(char *command, char **args, char **env);

#endif